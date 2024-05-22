#include "HrothCore_pch.hpp"

#include "HrothCore/Types/Camera.hpp"

#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Core/Window.hpp"
#include "HrothCore/Core/Input.hpp"

namespace HrothCore
{
    CameraPositionerEditor::CameraPositionerEditor(const Movement &movement, const KeyBinding &keyBinding)
        : m_Movement(movement), m_KeyBinding(keyBinding)
    {
        UpdateViewMatrix();
    }

    void CameraPositionerEditor::Update(float dt)
    {   
        if (Input::IsMouseButtonPressed(m_KeyBinding.Rotate)) {
            Window &window = Application::Get().GetWindow();
            if (m_IsMouseClicked == false) {
                m_IsMouseClicked = true;
                window.HideCursor();
                m_PrevMousePos = Input::GetMousePosition();
            }
            glm::vec2 currentMousePos = Input::GetMousePosition();
            const glm::vec2 delta = currentMousePos - m_PrevMousePos;

            if (glm::length(delta) > 0.0f)
            {
                m_Rotation.y += delta.x * m_Movement.RotationSpeed * dt;
                m_Rotation.y = glm::mod(m_Rotation.y, 360.0f);

                m_Rotation.x += delta.y * m_Movement.RotationSpeed * dt;
                m_Rotation.x = glm::clamp(m_Rotation.x, -80.0f, 80.0f);

                m_Dirty = true;
                window.SetCursorPos(m_PrevMousePos.x, m_PrevMousePos.y);
            }
        } else if (Input::IsMouseButtonReleased(m_KeyBinding.Rotate)) {
            m_IsMouseClicked = false;
            Window &window = Application::Get().GetWindow();
            window.ShowCursor();
        }
        
        glm::vec3 velocity(0.0f);

        if (Input::IsKeyPressed(m_KeyBinding.Forward)) velocity -= GetForward();
        if (Input::IsKeyPressed(m_KeyBinding.Backward)) velocity += GetForward();

        if (Input::IsKeyPressed(m_KeyBinding.Left)) velocity -= GetRight();
        if (Input::IsKeyPressed(m_KeyBinding.Right)) velocity += GetRight();

        if (Input::IsKeyPressed(m_KeyBinding.Up)) velocity += GetUp();
        if (Input::IsKeyPressed(m_KeyBinding.Down)) velocity -= GetUp();

        float ActualMaxSpeed = m_Movement.Speed;
        if (Input::IsKeyPressed(m_KeyBinding.Fast)) ActualMaxSpeed *= m_Movement.FastMultiplier;

        if (glm::length(velocity) > 0.0f)
        {
            m_Position += glm::normalize(velocity) * ActualMaxSpeed * dt;
            m_Dirty = true;
        }
    }

    void CameraPositionerEditor::SetPosition(const glm::vec3 &position)
    {
        m_Position = position;
        m_Dirty = true;
    }

    void CameraPositionerEditor::LookAt(const glm::vec3 &target)
    {
        const glm::vec3 direction = glm::normalize(target - m_Position);
        m_Rotation.x = glm::degrees(asin(-direction.y));
        m_Rotation.y = glm::degrees(atan2(direction.z, direction.x)) + 90.0f;

        m_Dirty = true;
    }

    glm::vec3 CameraPositionerEditor::GetForward()
    {
        auto view = GetViewMatrix();

        return glm::vec3(view[0][2], view[1][2], view[2][2]);
    }

    glm::vec3 CameraPositionerEditor::GetRight()
    {
        auto view = GetViewMatrix();

        return glm::vec3(view[0][0], view[1][0], view[2][0]);
    }

    glm::vec3 CameraPositionerEditor::GetUp()
    {
        return glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::mat4 CameraPositionerEditor::GetViewMatrix()
    {
        if (m_Dirty)
            UpdateViewMatrix();

        return m_ViewMatrix;
    }

    void CameraPositionerEditor::UpdateViewMatrix()
    {
        glm::quat qPitch = glm::angleAxis(glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat qYaw = glm::angleAxis(glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat orientation = glm::normalize(qPitch * qYaw);

        m_ViewMatrix = glm::translate(glm::mat4_cast(orientation), -m_Position);

        m_Dirty = false;
    }
}

// VIEW MATRIX VIZUALIZATION
// ///////////////////////////////////////////////
// |    RIGHT.x     RIGHT.y     RIGHT.z     0    |
// |    UP.x        UP.y        UP.z        0    |
// |    FORWARD.x   FORWARD.y   FORWARD.z   0    |
// |    POS.x       POS.y       POS.z       1    |
// ///////////////////////////////////////////////