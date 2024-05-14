#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Renderer.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace HrothCore
{
    void Renderer::Init()
    {
    }

    void Renderer::Shutdown()
    {
    }

    /* 3D */

    void Renderer::BeginDrawing3D(int cameraPersp)
    {
        HC_ASSERT(m_CurrentRenderMode == RenderMode::NONE);

        m_CurrentRenderMode = RenderMode::RENDER_3D;
    }

    void Renderer::DrawItem(int data3d)
    {
    }


    void Renderer::Submit(int data3d)
    {
    }

    void Renderer::MultiSubmit(std::vector<int> data3d)
    {
    }

    void Renderer::EndDrawing3D()
    {
        HC_ASSERT(m_CurrentRenderMode == RenderMode::RENDER_3D);

        m_CurrentRenderMode = RenderMode::NONE;
    }

    /* 2D */

    void Renderer::BeginDrawing2D(int cameraOrtho)
    {
        HC_ASSERT(m_CurrentRenderMode == RenderMode::NONE);

        m_CurrentRenderMode = RenderMode::RENDER_2D;
    }

    void Renderer::DrawItem(float data2d)
    {
    }


    void Renderer::Submit(float data2d)
    {
    }

    void Renderer::MultiSubmit(std::vector<float> data3d)
    {
    }

    void Renderer::EndDrawing2D()
    {
        HC_ASSERT(m_CurrentRenderMode == RenderMode::RENDER_2D);

        m_CurrentRenderMode = RenderMode::NONE;
    }
}
