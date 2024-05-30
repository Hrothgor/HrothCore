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

            TransformComponent(std::function<void()> onTransformChange)
                : OnTransformChange(onTransformChange)
            {
                OnTransformChange();
            }

        private:
            std::function<void()> OnTransformChange;

            glm::vec3 Position = glm::vec3(0.0f);
            glm::vec3 ScaleFactor = glm::vec3(1.0f);
            glm::vec3 Rotation = glm::vec3(0.0f);
    };

    struct TransformIsDirtyComponent
    {
        bool IsDirty;
    };
}