#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Input.hpp"
#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Core/Window.hpp"

#include <GLFW/glfw3.h>

namespace HrothCore
{
    bool Input::IsKeyPressed(KeyCode keyCode)
    {
        GLFWwindow *window = Application::Get().GetWindow().GetNativeWindow();
        return glfwGetKey(window, keyCode) == GLFW_PRESS;
    }

    bool Input::IsKeyReleased(KeyCode keyCode)
    {
        GLFWwindow *window = Application::Get().GetWindow().GetNativeWindow();
        return glfwGetKey(window, keyCode) == GLFW_RELEASE;
    }

    bool Input::IsMouseButtonPressed(MouseButton mouseCode)
    {
        GLFWwindow *window = Application::Get().GetWindow().GetNativeWindow();
        return glfwGetMouseButton(window, mouseCode) == GLFW_PRESS;
    }

    bool Input::IsMouseButtonReleased(MouseButton mouseCode)
    {
        GLFWwindow *window = Application::Get().GetWindow().GetNativeWindow();
        return glfwGetMouseButton(window, mouseCode) == GLFW_RELEASE;
    }

    glm::vec2 Input::GetMousePosition()
    {
        GLFWwindow *window = Application::Get().GetWindow().GetNativeWindow();
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return glm::vec2(xpos, ypos);
    }
}