#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Core/Window.hpp"
#include "HrothCore/Core/Engine.hpp"
#include "HrothCore/Core/IClient.hpp"

#include "HrothCore/Events/EventManager.hpp"
#include "HrothCore/Events/WindowEvent.hpp"
#include "HrothCore/Events/KeyMouseEvent.hpp"

namespace HrothCore
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification &specification, std::shared_ptr<IClient> &client)
		: m_Specification(specification)
	{
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

    	HrothCore::Logger::Init(m_Specification.LogFile);

		HC_ASSERT(s_Instance == nullptr, "Application already exists!");
		s_Instance = this;

		m_Window = std::make_unique<Window>(WindowProps(m_Specification.Name, 1280, 720, false));
		HC_REGISTER_EVENT(WindowCloseEvent, [](const WindowCloseEvent &event) -> bool
        {
            Application::Get().Close();
            return true;
        });
		HC_REGISTER_EVENT(KeyReleasedEvent, [](const KeyReleasedEvent &event) -> bool
        {
			if (event.Code == KeyCode::Escape)
			{
				Application::Get().Close();
			}
			return true;
        });

		Engine::Get().Init(client);
	}

	Application::~Application()
	{
		Engine::Get().Shutdown();
	}

	void Application::Close()
	{
		m_Window->Close();
	}

	void Application::Run()
	{
		while (m_Window->ShouldClose() == false)
		{
			float dt = static_cast<float>(m_Window->GetDeltaTime());

			Engine::Get().Update(dt);

            m_Window->Update();
		}
	}
}