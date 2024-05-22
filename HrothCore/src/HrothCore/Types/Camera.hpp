#pragma once

#include "HrothCore/Core/Input.hpp"

namespace HrothCore
{
    class ICameraPostioner
    {
    public:
        virtual ~ICameraPostioner() = default;

    private:
        virtual glm::mat4 GetViewMatrix() = 0;
        virtual glm::vec3 GetPosition() = 0;

        friend class Camera;
    };

    class Camera
    {
    public:
        Camera(ICameraPostioner &positioner, const float fov = 45.0f, const float near = 0.1f, const float far = 1000.0f)
            : m_FOV(fov), m_Near(near), m_Far(far)
        {
            m_Positioner = &positioner;
        }
        ~Camera() = default;

        glm::mat4 GetViewMatrix() const { return m_Positioner->GetViewMatrix(); }
        glm::vec3 GetPosition() const { return m_Positioner->GetPosition(); }

        void SetNear(const float near) { m_Near = near; }
        void SetFar(const float far) { m_Far = far; }
        void SetFOV(const float fov) { m_FOV = fov; }
        glm::mat4 GetProjMatrix(const float aspectRatio) const { return glm::perspective(m_FOV, aspectRatio, m_Near, m_Far); }

        private:
            ICameraPostioner *m_Positioner;
            float m_FOV = 45.0f;
            float m_Near = 0.1f;
            float m_Far = 1000.0f;
    };


    class CameraPositionerEditor : public ICameraPostioner
    {
        struct Movement {
            float Speed = 30.0f;
            float RotationSpeed = 90.0f;

            float FastMultiplier = 4.0f;
        };

        struct KeyBinding {
            KeyCode Forward = KeyCode::W;
            KeyCode Backward = KeyCode::S;
            KeyCode Left = KeyCode::A;
            KeyCode Right = KeyCode::D;
            KeyCode Up = KeyCode::E;
            KeyCode Down = KeyCode::Q;
            KeyCode Fast = KeyCode::LeftShift;

            MouseButton Rotate = MouseButton::ButtonLeft;
        };

    public:
        CameraPositionerEditor(const Movement &movement = {}, const KeyBinding &keyBinding = {});

        void Update(double dt);

        void SetPosition(const glm::vec3 &position);
        void LookAt(const glm::vec3 &target);

    private:
        glm::mat4 GetViewMatrix() override;
        glm::vec3 GetPosition() override { return m_Position; }

        glm::vec3 GetForward();
        glm::vec3 GetRight();
        glm::vec3 GetUp();

        void UpdateViewMatrix();
        
        glm::vec3 m_Position = glm::vec3(0.0f);
        glm::vec3 m_Rotation = glm::vec3(0.0f);

        glm::mat4 m_ViewMatrix;
        bool m_Dirty = false;

        bool m_IsMouseClicked = false;
        glm::vec2 m_PrevMousePos;

        Movement m_Movement;
        KeyBinding m_KeyBinding;
    };
}