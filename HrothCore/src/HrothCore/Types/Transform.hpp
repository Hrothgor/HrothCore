#pragma once

namespace HrothCore
{
    class Transform
    {
        public:
            Transform(const glm::vec3 &position);
            Transform(const glm::vec3 &position, const glm::vec3 &rotation);
            Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale);
            Transform();
            ~Transform();

            void SetPosition(const glm::vec3 &position);
            void Translate(const glm::vec3 &translation);

            void SetRotation(const glm::vec3 &rotation);
            void Rotate(const glm::vec3 &rotation);

            void SetScale(const glm::vec3 &scale);
            void Scale(const glm::vec3 &scale);

            const glm::vec3 &GetPosition() const { return m_Position; };
            const glm::vec3 &GetRotation() const { return m_Rotation; };
            const glm::vec3 &GetScale() const { return m_Scale; }

            glm::vec3 GetForward();
            glm::vec3 GetRight();
            glm::vec3 GetUp();

            const glm::mat4 &GetTransformMatrix();

        private:
            void UpdateTransformMatrix();

            glm::vec3 m_Position;
            glm::vec3 m_Rotation;
            glm::vec3 m_Scale;

            glm::mat4 m_TransformMatrix;
            bool m_Dirty = true;
    };
}

// MATRIX VIZUALIZATION
// ///////////////////////////////////////////////
// |    RIGHT.x     RIGHT.y     RIGHT.z     0    |
// |    UP.x        UP.y        UP.z        0    |
// |    FORWARD.x   FORWARD.y   FORWARD.z   0    |
// |    POS.x       POS.y       POS.z       1    |
// ///////////////////////////////////////////////