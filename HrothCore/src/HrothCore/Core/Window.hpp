#pragma once

struct GLFWwindow;

namespace HrothCore
{
    struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool VSync;

		WindowProps(const std::string& title = "Hazel Engine",
			        uint32_t width = 1600,
			        uint32_t height = 900,
					bool VSync = true)
			: Title(title), Width(width), Height(height), VSync(VSync)
		{
		}
	};

    class Window
    {
    public:
        Window(const WindowProps &props = WindowProps());
        virtual ~Window();

        void Update();

		uint32_t GetWidth() const { return m_Props.Width; }
		uint32_t GetHeight() const { return m_Props.Height; }

		void EnableVSync(bool enable = true);
		bool IsVSync() const { return m_Props.VSync; }

	private:
		void Init(const WindowProps &props);
		void Shutdown();

    private:
        WindowProps m_Props;
		GLFWwindow *m_Window;
    };
}