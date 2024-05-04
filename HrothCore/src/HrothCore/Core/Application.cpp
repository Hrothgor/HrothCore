#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Application.hpp"

namespace HrothCore {
	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		// Set working directory here
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);
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
            std::cout << "test" << std::endl;
		}
	}
}