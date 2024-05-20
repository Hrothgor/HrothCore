#include "HrothCore_pch.hpp"

#include "HrothCore/Types/Camera.hpp"

#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Core/Window.hpp"
#include "HrothCore/Core/Input.hpp"

namespace HrothCore
{
    CameraPositionerFirstPerson::CameraPositionerFirstPerson(const FirstPersonMovement &movement)
        : m_Movement(movement)
    {
        UpdateViewMatrix();
    }

    void CameraPositionerFirstPerson::Update(double dt)
    {   
        float fdt = static_cast<float>(dt);

        if (Input::IsMouseButtonPressed(MouseButton::ButtonLeft)) {
            Window &window = Application::Get().GetWindow();
            if (m_IsMouseClicked == false) {
                m_IsMouseClicked = true;
                window.HideCursor();
                m_MousePos = Input::GetMousePosition();
            }
            glm::vec2 currentMousePos = Input::GetMousePosition();
            const glm::vec2 delta = currentMousePos - m_MousePos;

            const glm::quat deltaQuat = glm::quat(glm::vec3(m_Movement.RotationSpeed * delta.y * fdt, 
                                                            m_Movement.RotationSpeed * delta.x * fdt,
                                                            0.0f));

            m_Orientation = deltaQuat * m_Orientation;
            m_Orientation = glm::normalize(m_Orientation);
            m_Dirty = true;

            window.SetCursorPos(m_MousePos.x, m_MousePos.y);
        } else if (Input::IsMouseButtonReleased(MouseButton::ButtonLeft)) {
            m_IsMouseClicked = false;
            Window &window = Application::Get().GetWindow();
            window.ShowCursor();
        }
        
        const glm::mat4 view = GetViewMatrix();

        const glm::vec3 forward = glm::vec3(view[0][2], view[1][2], view[2][2]);
        const glm::vec3 right = glm::vec3(view[0][0], view[1][0], view[2][0]);
        const glm::vec3 up = glm::vec3(view[0][1], view[1][1], view[2][1]);

        glm::vec3 accel = glm::vec3(0.0f);

        if (Input::IsKeyPressed(KeyCode::W)) accel += forward;
        if (Input::IsKeyPressed(KeyCode::S)) accel -= forward;

        if (Input::IsKeyPressed(KeyCode::D)) accel += right;
        if (Input::IsKeyPressed(KeyCode::Q)) accel -= right;

        if (Input::IsKeyPressed(KeyCode::E)) accel += up;
        if (Input::IsKeyPressed(KeyCode::A)) accel -= up;

        float ActualMaxSpeed = m_Movement.MaxSpeed;
        if (Input::IsKeyPressed(KeyCode::LeftShift))
        {
            accel *= m_Movement.FastMultiplier;
            ActualMaxSpeed *= m_Movement.FastMultiplier;
        }

        if (glm::length(accel) > 0.0f)
        {
            m_Velocity += glm::normalize(accel) * m_Movement.Acceleration * fdt;
            if (glm::length(m_Velocity) > ActualMaxSpeed)
                m_Velocity = glm::normalize(m_Velocity) * ActualMaxSpeed;
            m_Position += m_Velocity * fdt;
            m_Dirty = true;
        }
    }

    void CameraPositionerFirstPerson::SetPosition(const glm::vec3 &position)
    {
        m_Position = position;
        m_Dirty = true;
    }

    void CameraPositionerFirstPerson::SetUpVector(const glm::vec3 &up)
    {
        m_Up = up;
        const glm::mat4 view = GetViewMatrix();
        const glm::vec3 dir = glm::vec3(view[0][2], view[1][2], view[2][2]);
        m_Orientation = glm::lookAt(m_Position, m_Position + dir, m_Up);
        m_Dirty = true;
    }

    void CameraPositionerFirstPerson::LookAt(const glm::vec3 &target)
    {
        m_Orientation = glm::lookAt(m_Position, target, m_Up);
        m_Dirty = true;
    }

    glm::mat4 CameraPositionerFirstPerson::GetViewMatrix()
    {
        if (m_Dirty)
        {
            UpdateViewMatrix();
            m_Dirty = false;
        }

        return m_ViewMatrix;
    }

    void CameraPositionerFirstPerson::UpdateViewMatrix()
    {
        glm::mat4 matrix(1.0f);
        matrix = glm::translate(matrix, -m_Position);
        matrix = glm::mat4_cast(m_Orientation) * matrix;
        m_ViewMatrix = matrix;
    }
}

// VIEW MATRIX VIZUALIZATION
// ///////////////////////////////////////////////
// |    RIGHT.x     RIGHT.y     RIGHT.z     0    |
// |    UP.x        UP.y        UP.z        0    |
// |    FORWARD.x   FORWARD.y   FORWARD.z   0    |
// |    POS.x       POS.y       POS.z       1    |
// ///////////////////////////////////////////////