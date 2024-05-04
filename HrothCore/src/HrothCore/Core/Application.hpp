#pragma once

int main(int argc, char** argv);

namespace HrothCore {
	class Window;
	
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			// ASSERT index < Count
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "HrothCore Application";
		std::string WorkingDirectory = "./";
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void Close();

		static Application& Get() { return *s_Instance; }
	private:
		void Run();

	private:
		ApplicationSpecification m_Specification;
		bool m_Running = true;

		std::unique_ptr<Window> m_Window;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs args);
}