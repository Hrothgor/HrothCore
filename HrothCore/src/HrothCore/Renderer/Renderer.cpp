#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Renderer.hpp"
#include "HrothCore/Renderer/RenderCommand.hpp"
#include "HrothCore/Renderer/VertexArray.hpp"
#include "HrothCore/Renderer/Buffer.hpp"
#include "HrothCore/Renderer/Shader.hpp"
#include "HrothCore/Renderer/Texture.hpp"

#include "HrothCore/Types/Transform.hpp"
#include "HrothCore/Types/Camera.hpp"

#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Core/Window.hpp"
#include "HrothCore/Core/AssetManager.hpp"
#include "HrothCore/Events/WindowEvent.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    namespace Renderer {
        void StartBatch();
        void NextBatch();
        void Flush();
    }

    struct RenderData {
        /* Constexpr */
        static constexpr uint32_t MaxMeshes = 4096;
        static constexpr uint32_t MaxTextures = 4 * MaxMeshes;
        /* --------- */

        /* VAO/SHADER/SSBO */
        std::shared_ptr<VertexArray> MeshVAO;
        std::array<std::shared_ptr<Shader>, NumShaders> Shaders;

        std::shared_ptr<Buffer<uint32_t>> BufferBindlessSamplers;
        /* --------------- */

        /* PerMeshData */
        struct PerMeshData {
            glm::mat4 transform;
        };
        std::shared_ptr<Buffer<PerMeshData>> BufferPerMeshData;
        uint32_t BatchMeshCount = 0;
        PerMeshData *BufferPerMeshDataBase = nullptr;
        PerMeshData *BufferPerMeshDataPtr = nullptr;
        /* ----------- */

        /* Indirect Buffer */
        struct DrawIndirectCommand {
            uint32_t indexCount;
            uint32_t instanceCount;
            uint32_t firstIndex;
            uint32_t baseVertex;
            uint32_t baseInstance;
        };
        std::shared_ptr<Buffer<DrawIndirectCommand>> BufferIndirectDraw;
        DrawIndirectCommand *BufferIndirectDrawBase = nullptr;
        DrawIndirectCommand *BufferIndirectDrawPtr = nullptr;
        /* -------------- */

        /* PerFrameData */
        struct PerFrameData {
            glm::mat4 view;
            glm::mat4 proj;
        };
        std::shared_ptr<Buffer<PerFrameData>> BufferPerFrameData;
        PerFrameData PerFrameDataUniform;
        /* ------------ */

        glm::ivec2 FramebufferSize;
    };

    static RenderData s_Data;

    void Renderer::Init()
    {
        s_Data.FramebufferSize.x = Application::Get().GetWindow().GetWidth();
        s_Data.FramebufferSize.y = Application::Get().GetWindow().GetHeight();
		HC_REGISTER_EVENT(WindowResizeEvent, [](const WindowResizeEvent &event) -> bool
        {
            s_Data.FramebufferSize.x = event.Width;
            s_Data.FramebufferSize.y = event.Height;
            return true;
        });

        s_Data.Shaders[MeshShader] = std::make_shared<Shader>("./assets/shaders/Basic.vert", "./assets/shaders/Basic.frag");

        s_Data.MeshVAO = std::make_shared<VertexArray>();

        s_Data.BufferBindlessSamplers = std::make_shared<Buffer<uint32_t>>(RenderData::MaxTextures);

        s_Data.BufferPerMeshData = std::make_shared<Buffer<RenderData::PerMeshData>>(RenderData::MaxMeshes);
        s_Data.BufferPerMeshData->BindToShader(0, BufferShaderType::ShaderStorage);
        s_Data.BufferPerMeshDataBase = new RenderData::PerMeshData[RenderData::MaxMeshes];

        s_Data.BufferIndirectDraw = std::make_shared<Buffer<RenderData::DrawIndirectCommand>>(RenderData::MaxMeshes);
        s_Data.BufferIndirectDraw->BindIndirectDraw();
        s_Data.BufferIndirectDrawBase = new RenderData::DrawIndirectCommand[RenderData::MaxMeshes];

        s_Data.BufferPerFrameData = std::make_shared<Buffer<RenderData::PerFrameData>>();
        s_Data.BufferPerFrameData->BindToShader(0, BufferShaderType::Uniform);
    }

    void Renderer::Shutdown()
    {
        delete s_Data.BufferPerMeshDataBase;
    }

    void Renderer::BeginScene(const Camera &camera)
    {
        RenderCommand::EnableDepthTest(true);
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        RenderCommand::Clear();

        RenderCommand::SetViewport(s_Data.FramebufferSize);

        s_Data.PerFrameDataUniform.view = camera.GetViewMatrix();
        s_Data.PerFrameDataUniform.proj = camera.GetProjMatrix(s_Data.FramebufferSize.x / (float)s_Data.FramebufferSize.y);

        StartBatch();
    }

    void Renderer::EndScene()
    {
        Flush();
    }

    void Renderer::DrawMesh(const Mesh &mesh, const glm::mat4 &transform)
    {
        if (s_Data.BatchMeshCount >= RenderData::MaxMeshes)
            NextBatch();

        s_Data.BufferPerMeshDataPtr->transform = transform;
        s_Data.BufferPerMeshDataPtr++;

        s_Data.BufferIndirectDrawPtr->indexCount = mesh.IndicesCount;
        s_Data.BufferIndirectDrawPtr->instanceCount = 1;
        s_Data.BufferIndirectDrawPtr->firstIndex = mesh.BaseIndex;
        s_Data.BufferIndirectDrawPtr->baseVertex = mesh.BaseVertex;
        s_Data.BufferIndirectDrawPtr->baseInstance = 0;
        s_Data.BufferIndirectDrawPtr++;

        s_Data.BatchMeshCount++;
    }

    /* GPU LOADING */

    std::pair<int, int> Renderer::LoadVertexData(VerticesData vertices, std::vector<uint32_t> indices)
    {
        std::pair<int, int> offsets = std::make_pair(s_Data.MeshVAO->GetVerticesCount(), s_Data.MeshVAO->GetIndicesCount());
        s_Data.MeshVAO->AddVertices(vertices, indices);
        return offsets;
    }

    void Renderer::LoadBindlessTexture(const Texture &texture)
    {
        uint32_t bindlessID = texture.GetBindlessID();
        s_Data.BufferBindlessSamplers->AddData(1, &bindlessID);
    }

    /* ----------- */

    /* Utils */

    void Renderer::ReloadShader(Shaders shader)
    {
        s_Data.Shaders[shader]->Hotreload();
    }

    void Renderer::ReloadAllShader()
    {
        for (auto &shader : s_Data.Shaders)
            shader->Hotreload();
    }

    /* ----- */

    void Renderer::StartBatch()
    {
        s_Data.BatchMeshCount = 0;
        s_Data.BufferPerMeshDataPtr = s_Data.BufferPerMeshDataBase;
        s_Data.BufferIndirectDrawPtr = s_Data.BufferIndirectDrawBase;
    }

    void Renderer::NextBatch()
    {
        Flush();
        StartBatch();
    }

    void Renderer::Flush()
    {
        s_Data.BufferPerFrameData->SetData(1, &s_Data.PerFrameDataUniform);

        if (s_Data.BatchMeshCount)
        {
            s_Data.BufferPerMeshData->SetData(s_Data.BatchMeshCount, s_Data.BufferPerMeshDataBase);
            s_Data.BufferIndirectDraw->SetData(s_Data.BatchMeshCount, s_Data.BufferIndirectDrawBase);

            s_Data.MeshVAO->Bind();
            s_Data.Shaders[MeshShader]->Start();
            glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, s_Data.BatchMeshCount, 0);
        }
    }
}