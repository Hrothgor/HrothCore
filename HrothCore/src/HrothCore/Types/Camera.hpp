#pragma once

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
        Camera(ICameraPostioner &positioner)
        {
            m_Positioner = &positioner;
        }
        ~Camera() = default;

        glm::mat4 GetViewMatrix() const { return m_Positioner->GetViewMatrix(); }
        glm::vec3 GetPosition() const { return m_Positioner->GetPosition(); }

        private:
            ICameraPostioner *m_Positioner;
    };

    struct FirstPersonMovement {
        float MaxSpeed = 30.0f;
        float Acceleration = 150.0f;

        float RotationSpeed = 20.0;

        float FastMultiplier = 4.0;
    };

    class CameraPositionerFirstPerson : public ICameraPostioner
    {
    public:
        CameraPositionerFirstPerson(const FirstPersonMovement &movement = {});

        void Update(double dt);

        void SetPosition(const glm::vec3 &position);
        void SetUpVector(const glm::vec3 &up);
        void LookAt(const glm::vec3 &target);


    private:
        glm::mat4 GetViewMatrix() override;
        glm::vec3 GetPosition() override { return m_Position; }

        void UpdateViewMatrix();
        
        glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 m_Position = glm::vec3(0.0f);
        glm::quat m_Orientation = glm::quat(glm::vec3(0.0f));

        glm::vec3 m_Velocity = glm::vec3(0.0f);

        bool m_IsMouseClicked = false;
        glm::vec2 m_MousePos;

        glm::mat4 m_ViewMatrix;
        bool m_Dirty = true;

        FirstPersonMovement m_Movement;
    };
}