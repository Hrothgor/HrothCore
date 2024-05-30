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
        OnTransformChange();
    }

    void TransformComponent::SetScale(const glm::vec3 &scale)
    {
        ScaleFactor = scale;
        OnTransformChange();
    }

    void TransformComponent::SetRotation(const glm::vec3 &rotation)
    {
        Rotation = rotation;
        OnTransformChange();
    }

    void TransformComponent::Translate(const glm::vec3 &translation)
    {
        SetPosition(translation + GetPosition());
    }

    void TransformComponent::Scale(const glm::vec3 &scale)
    {
        SetScale(scale * GetScale());
    }

    void TransformComponent::Rotate(const glm::vec3 &rotation)
    {
        SetRotation(rotation + GetRotation());
    }
}