#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Renderer.hpp"
#include "HrothCore/Renderer/RenderCommand.hpp"
#include "HrothCore/Renderer/VertexArray.hpp"
#include "HrothCore/Renderer/Buffer.hpp"
#include "HrothCore/Renderer/Framebuffer.hpp"
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

        /* GPU stuff */
        std::shared_ptr<VertexArray> MeshVAO;
        std::shared_ptr<VertexArray> EmptyVAO;
        std::array<std::shared_ptr<Shader>, NumShaders> Shaders;
        std::array<std::shared_ptr<Framebuffer>, NumFramebuffers> Framebuffers;

        std::shared_ptr<Buffer<uint64_t>> BufferBindlessSamplers;
        /* --------- */

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
            glm::vec2 resolution;
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

        s_Data.MeshVAO = std::make_shared<VertexArray>();
        s_Data.EmptyVAO = std::make_shared<VertexArray>();

        s_Data.Shaders[GBufferShader] = std::make_shared<Shader>("./assets/shaders/GBuffer.vert", "./assets/shaders/GBuffer.frag");
        s_Data.Shaders[ScreenViewShader] = std::make_shared<Shader>("./assets/shaders/View.vert", "./assets/shaders/View.frag");
        
        s_Data.Framebuffers[GBuffer] = std::make_shared<Framebuffer>(s_Data.FramebufferSize.x, s_Data.FramebufferSize.y);
        s_Data.Framebuffers[GBuffer]->CreateTextureAttachment("Tex1", TextureInfo{ .dataType = TextureInfo::DataType::UByte });
        s_Data.Framebuffers[GBuffer]->CreateTextureAttachment("Tex2", TextureInfo{ .dataType = TextureInfo::DataType::UByte });
        s_Data.Framebuffers[GBuffer]->CreateTextureAttachment("Tex3", TextureInfo{ .dataType = TextureInfo::DataType::UByte });
        s_Data.Framebuffers[GBuffer]->CreateTextureAttachment("Tex4", TextureInfo{ .dataType = TextureInfo::DataType::UByte });
        s_Data.Framebuffers[ScreenView] = std::make_shared<Framebuffer>(s_Data.FramebufferSize.x, s_Data.FramebufferSize.y);
        s_Data.Framebuffers[ScreenView]->CreateTextureAttachment("Color", TextureInfo{ .dataType = TextureInfo::DataType::UByte });

        s_Data.BufferBindlessSamplers = std::make_shared<Buffer<uint64_t>>(RenderData::MaxTextures);

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
        s_Data.EmptyVAO->Release();
        s_Data.MeshVAO->Release();

        for (auto &shader : s_Data.Shaders)
            shader->Release();

        for (auto &framebuffer : s_Data.Framebuffers)
            framebuffer->Release();

        s_Data.BufferBindlessSamplers->Release();

        s_Data.BufferPerMeshData->Release();
        delete s_Data.BufferPerMeshDataBase;

        s_Data.BufferIndirectDraw->Release();
        delete s_Data.BufferIndirectDrawBase;
    
        s_Data.BufferPerFrameData->Release();
    }

    void Renderer::BeginScene(const Camera &camera)
    {
        RenderCommand::EnableDepthTest(true);

        s_Data.PerFrameDataUniform.view = camera.GetViewMatrix();
        s_Data.PerFrameDataUniform.proj = camera.GetProjMatrix(s_Data.FramebufferSize.x / (float)s_Data.FramebufferSize.y);
        s_Data.PerFrameDataUniform.resolution = glm::vec2(s_Data.FramebufferSize);

        s_Data.Framebuffers[GBuffer]->Clear();
        s_Data.Framebuffers[ScreenView]->Clear();

        StartBatch();
    }

    void Renderer::EndScene()
    {
        Flush();

        // ScreenView pass
        s_Data.Framebuffers[ScreenView]->BindForDrawing();
        s_Data.Shaders[ScreenViewShader]->Start();
        s_Data.EmptyVAO->Bind();
        s_Data.Framebuffers[GBuffer]->GetTexture("Tex1")->BindTextureUnit(0);
        s_Data.Framebuffers[GBuffer]->GetTexture("Tex2")->BindTextureUnit(1);
        s_Data.Framebuffers[GBuffer]->GetTexture("Tex3")->BindTextureUnit(2);
        s_Data.Framebuffers[GBuffer]->GetTexture("Tex4")->BindTextureUnit(3);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, s_Data.FramebufferSize.x, s_Data.FramebufferSize.y);
    }

    void Renderer::RenderToScreen()
    {
        // Blit to screen
        glBlitNamedFramebuffer(s_Data.Framebuffers[ScreenView]->GetID(), 0, 0, 0, s_Data.FramebufferSize.x, s_Data.FramebufferSize.y, 0, 0, s_Data.FramebufferSize.x, s_Data.FramebufferSize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
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
        uint64_t bindlessID = texture.GetBindlessID();
        s_Data.BufferBindlessSamplers->AddData(1, &bindlessID);
    }

    /* ----------- */

    /* Utils */

    std::shared_ptr<Framebuffer> Renderer::GetFramebuffer(Framebuffers framebuffer)
    {
        if (framebuffer >= NumFramebuffers)
        {
            HC_LOG_WARNING("Renderer::GetFramebuffer: Framebuffer index out of range");
            return nullptr;
        }

        return s_Data.Framebuffers[framebuffer];
    }

    std::shared_ptr<Shader> Renderer::GetShader(Shaders shader)
    {
        if (shader >= NumShaders)
        {
            HC_LOG_WARNING("Renderer::GetShader: Shader index out of range");
            return nullptr;
        }

        return s_Data.Shaders[shader];
    }

    void Renderer::ReloadShader(Shaders shader)
    {
        if (shader >= NumShaders)
        {
            HC_LOG_WARNING("Renderer::ReloadShader: Shader index out of range");
            return;
        }

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

        // GBuffer pass
        if (s_Data.BatchMeshCount)
        {
            s_Data.BufferPerMeshData->SetData(s_Data.BatchMeshCount, s_Data.BufferPerMeshDataBase);
            s_Data.BufferIndirectDraw->SetData(s_Data.BatchMeshCount, s_Data.BufferIndirectDrawBase);

            s_Data.Framebuffers[GBuffer]->BindForDrawing();
            s_Data.Shaders[GBufferShader]->Start();
            s_Data.MeshVAO->Bind();
            glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, s_Data.BatchMeshCount, 0);
        }
    }
}