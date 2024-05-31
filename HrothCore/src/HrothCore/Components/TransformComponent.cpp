#include "HrothCore_pch.hpp"

#include "HrothCore/Components/TransformComponent.hpp"

namespace HrothCore
{
    glm::vec3 TransformComponent::GetPosition() const
    {
        return Position;
    }

    glm::vec3 TransformComponent::GetScale() const
    {
        return ScaleFactor;
    }

    glm::vec3 TransformComponent::GetRotation() const
    {
        return Rotation;
    }

    void TransformComponent::SetPosition(const glm::vec3 &position)
    {
        Position = position;
        IsDirty = true;
    }

    void TransformComponent::SetScale(const glm::vec3 &scale)
    {
        ScaleFactor = scale;
        IsDirty = true;
    }

    void TransformComponent::SetRotation(const glm::vec3 &rotation)
    {
        Rotation = rotation;
        IsDirty = true;
    }

    void TransformComponent::Translate(const glm::vec3 &translation)
    {
        SetPosition(translation + GetPosition());
        IsDirty = true;
    }

    void TransformComponent::Scale(const glm::vec3 &scale)
    {
        SetScale(scale * GetScale());
        IsDirty = true;
    }

    void TransformComponent::Rotate(const glm::vec3 &rotation)
    {
        SetRotation(rotation + GetRotation());
        IsDirty = true;
    }

    void TransformComponent::UpdateLocal()
    {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, Position);
        matrix = matrix * glm::mat4_cast(glm::quat(glm::radians(Rotation)));
        matrix = glm::scale(matrix, ScaleFactor);
        Local = matrix;
    }
}