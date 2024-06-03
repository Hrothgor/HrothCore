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
        struct PerMeshData_t {
            glm::mat4 transform;
        };
        std::shared_ptr<Buffer<PerMeshData_t>> BufferPerMeshData;
        uint32_t MeshIndexCount = 0;
        PerMeshData_t *BufferPerMeshDataBase = nullptr;
        PerMeshData_t *BufferPerMeshDataPtr = nullptr;
        /* ----------- */

        /* PerFrameData */
        struct PerFrameData_t {
            glm::mat4 view;
            glm::mat4 proj;
        };
        std::shared_ptr<Buffer<PerFrameData_t>> BufferPerFrameData;
        PerFrameData_t PerFrameData;
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

        s_Data.BufferPerMeshData = std::make_shared<Buffer<RenderData::PerMeshData_t>>(RenderData::MaxMeshes);
        s_Data.BufferPerMeshDataBase = new RenderData::PerMeshData_t[RenderData::MaxMeshes];
        s_Data.BufferPerMeshData->BindToShader(0, BufferShaderType::ShaderStorage);

        s_Data.BufferPerFrameData = std::make_shared<Buffer<RenderData::PerFrameData_t>>();
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

        s_Data.PerFrameData.view = camera.GetViewMatrix();
        s_Data.PerFrameData.proj = camera.GetProjMatrix(s_Data.FramebufferSize.x / (float)s_Data.FramebufferSize.y);

        StartBatch();
    }

    void Renderer::EndScene()
    {
        Flush();
    }

    void Renderer::DrawMesh(const Mesh &mesh, const glm::mat4 &transform)
    {
        if (s_Data.MeshIndexCount >= RenderData::MaxMeshes)
            NextBatch();

        s_Data.BufferPerMeshDataPtr->transform = transform;
        s_Data.BufferPerMeshDataPtr++;

        s_Data.MeshIndexCount++;
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
        s_Data.MeshIndexCount = 0;
        s_Data.BufferPerMeshDataPtr = s_Data.BufferPerMeshDataBase;
    }

    void Renderer::NextBatch()
    {
        Flush();
        StartBatch();
    }

    void Renderer::Flush()
    {
        s_Data.BufferPerFrameData->SetData(1, &s_Data.PerFrameData);

        if (s_Data.MeshIndexCount)
        {
            s_Data.BufferPerMeshData->SetData(s_Data.MeshIndexCount, s_Data.BufferPerMeshDataBase);

            s_Data.MeshVAO->Bind();
            s_Data.Shaders[MeshShader]->Start();
            glDrawArraysInstanced(GL_TRIANGLES, 0, 36, s_Data.MeshIndexCount);
        }
    }
}