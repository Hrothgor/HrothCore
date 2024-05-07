#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Core/Window.hpp"
#include "HrothCore/Core/Engine.hpp"

#include "HrothCore/Events/EventManager.hpp"
#include "HrothCore/Events/WindowEvent.hpp"
#include "HrothCore/Events/KeyMouseEvent.hpp"

namespace HrothCore {
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		HC_ASSERT(s_Instance == nullptr);
		s_Instance = this;

		// Set working directory here
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		m_Window = std::make_unique<Window>(WindowProps(m_Specification.Name));

		HC_REGISTER_EVENT(WindowCloseEvent, [](const WindowCloseEvent& event) -> bool
        {
            Application::Get().Close();
            return true;
        });
	}

	Application::~Application()
	{
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			double dt = m_Window->GetDeltaTime();

			Engine::Get().Update(dt);

            m_Window->Update();
		}
	}
}