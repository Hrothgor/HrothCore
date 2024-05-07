#pragma once

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWvidmode;

namespace HrothCore
{
	enum class WindowMode
	{
		Windowed,
		Fullscreen,
	};

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

	struct WindowPosition
	{
		int x,y = 0;
	};

    class Window
    {
    public:
        Window(const WindowProps &props = WindowProps(), WindowMode mode = WindowMode::Windowed);
        virtual ~Window();

        void Update();

		uint32_t GetWidth() const { return m_Props.Width; }
		uint32_t GetHeight() const { return m_Props.Height; }

		void EnableVSync(bool enable = true);
		bool IsVSync() const { return m_Props.VSync; }

		void SetWindowMode(WindowMode mode);

		float GetDeltaTime() const { return m_DeltaTime; }

	private:
		void Init(const WindowProps &props);
		void Shutdown();

    private:
        WindowProps m_Props;
		float m_LastFrameTime = 0.0f;
		float m_DeltaTime = 0.016f;
		GLFWwindow *m_Window;
		GLFWmonitor *m_Monitor;
		const GLFWvidmode *m_VideoMode;

		int m_OldWindowedSize[2];
		int m_OldWindowedPos[2];
		WindowMode m_CurrentWindowMode = WindowMode::Windowed;
    };
}