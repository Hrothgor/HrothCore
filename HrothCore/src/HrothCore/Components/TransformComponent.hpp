#pragma once

namespace HrothCore
{
    struct TransformComponent
    {
        public:
            glm::mat4 Local = glm::mat4(1.0f);
            glm::mat4 Global = glm::mat4(1.0f);

            glm::vec3 GetPosition() const;
            glm::vec3 GetScale() const;
            glm::vec3 GetRotation() const;

            void SetPosition(const glm::vec3 &position);
            void SetScale(const glm::vec3 &scale);
            void SetRotation(const glm::vec3 &rotation);

            void Translate(const glm::vec3 &translation);
            void Scale(const glm::vec3 &scale);
            void Rotate(const glm::vec3 &rotation);

        private:
            void UpdateLocal();

            glm::vec3 Position = glm::vec3(0.0f);
            glm::vec3 ScaleFactor = glm::vec3(1.0f);
            glm::vec3 Rotation = glm::vec3(0.0f);

            bool IsDirty = true;

        friend class Scene;
    };
}