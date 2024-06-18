#pragma once

// For use by HrothCore applications

#include "HrothCore_pch.hpp"

// ---Core----------------------------
#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Core/Window.hpp"
#include "HrothCore/Core/IClient.hpp"
#include "HrothCore/Core/Engine.hpp"
#include "HrothCore/Core/AssetManager.hpp"
// -----------------------------------

// ---Renderer------------------------
#include "HrothCore/Renderer/Buffer.hpp"
#include "HrothCore/Renderer/Texture.hpp"
#include "HrothCore/Renderer/Framebuffer.hpp"
#include "HrothCore/Renderer/Shader.hpp"
#include "HrothCore/Renderer/VertexArray.hpp"
#include "HrothCore/Renderer/ImGuiLayer.hpp"
#include "HrothCore/Renderer/Renderer.hpp"
// -----------------------------------

// ---Scene---------------------------
#include "HrothCore/Scene/Scene.hpp"
#include "HrothCore/Scene/GameObject.hpp"
// -----------------------------------

// ---Events--------------------------
#include "HrothCore/Events/EventManager.hpp"
#include "HrothCore/Events/WindowEvent.hpp"
#include "HrothCore/Events/KeyMouseEvent.hpp"
// -----------------------------------

// ---Types---------------------------
#include "HrothCore/Types/Mesh.hpp"
#include "HrothCore/Types/Model.hpp"
#include "HrothCore/Types/Camera.hpp"
// -----------------------------------

// ---Components----------------------
#include "HrothCore/Components/IDComponent.hpp"
#include "HrothCore/Components/TransformComponent.hpp"
#include "HrothCore/Components/LightComponent.hpp"
#include "HrothCore/Components/StaticMeshComponent.hpp"
// -----------------------------------

// ---Entry Point---------------------
#include "HrothCore/Core/EntryPoint.hpp"
// -----------------------------------