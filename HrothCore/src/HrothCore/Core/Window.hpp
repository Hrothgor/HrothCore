#pragma once

#include "HrothCore/Renderer/RenderContext.hpp"

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWvidmode;

namespace HrothCore
{
	enum class WindowMode
	{
		Windowed,
		Fullscreen,
		Borderless,
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

    class Window
    {
    public:
        Window(const WindowProps &props = WindowProps(), WindowMode mode = WindowMode::Windowed);
        virtual ~Window();

        void Update();

		bool ShouldClose() const;
		void Close();

		uint32_t GetWidth() const { return m_Props.Width; }
		uint32_t GetHeight() const { return m_Props.Height; }

		void EnableVSync(bool enable = true);
		bool IsVSync() const { return m_Props.VSync; }

		void SetWindowMode(WindowMode mode);

		double GetDeltaTime() const { return m_DeltaTime; }

		GLFWwindow *GetNativeWindow() const { return m_Window; }

	private:
		void Init(const WindowProps &props);
		void SetupCallbacks();
		void Shutdown();

    private:
        WindowProps m_Props;
		std::unique_ptr<RenderContext> m_RenderContext;
		
		double m_LastFrameTime = 0.0f;
		double m_DeltaTime = 0.016f;

		GLFWwindow *m_Window;
		GLFWmonitor *m_Monitor;
		const GLFWvidmode *m_VideoMode;

		int m_OldWindowedSize[2];
		int m_OldWindowedPos[2];
		WindowMode m_CurrentWindowMode = WindowMode::Windowed;
    };
}