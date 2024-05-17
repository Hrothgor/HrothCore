#pragma once

int main(int argc, char** argv);

namespace HrothCore {
	class Window;
	class IClient;
	
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			HC_ASSERT(index < Count);
			HC_ASSERT(Args != nullptr);
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "HrothCore Application";
		std::string WorkingDirectory = "./";
		std::string LogFile = "./HrothCore.log";
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specificationn, std::shared_ptr<IClient>& client);
		virtual ~Application();

		void Close();
		Window &GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }
	private:
		void Run();

	private:
		ApplicationSpecification m_Specification;
		std::unique_ptr<Window> m_Window;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs args);
}