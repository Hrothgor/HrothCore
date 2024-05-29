#include "HrothCore_pch.hpp"

#include "HrothCore/Types/Transform.hpp"

namespace HrothCore
{
    Transform::Transform(const glm::vec3 &position)
        : m_Position(position), m_Rotation(0.0f), m_Scale(1.0f)
    {
        UpdateTransformMatrix();
    }

    Transform::Transform(const glm::vec3 &position, const glm::vec3 &rotation)
        : m_Position(position), m_Rotation(rotation), m_Scale(1.0f)
    {
        UpdateTransformMatrix();
    }

    Transform::Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
        : m_Position(position), m_Rotation(rotation), m_Scale(scale)
    {
        UpdateTransformMatrix();
    }

    Transform::Transform()
        : m_Position(0.0f), m_Rotation(0.0f), m_Scale(1.0f)
    {
        UpdateTransformMatrix();
    }

    Transform::~Transform()
    {
    }

    void Transform::SetPosition(const glm::vec3 &position)
    {
        m_Position = position;
        m_Dirty = true;
    }

    void Transform::Translate(const glm::vec3 &translation)
    {
        m_Position += translation;
        m_Dirty = true;
    }

    void Transform::SetRotation(const glm::vec3 &rotation)
    {
        m_Rotation = rotation;
        m_Dirty = true;
    }

    void Transform::Rotate(const glm::vec3 &rotation)
    {
        m_Rotation += rotation;
        m_Dirty = true;
    }

    void Transform::SetScale(const glm::vec3 &scale)
    {
        m_Scale = scale;
        m_Dirty = true;
    }

    void Transform::Scale(const glm::vec3 &scale)
    {
        m_Scale *= scale;
        m_Dirty = true;
    }

    glm::vec3 Transform::GetForward()
    {
        if (m_Dirty)
            UpdateTransformMatrix();

        return glm::vec3(m_TransformMatrix[2][0], m_TransformMatrix[2][1], m_TransformMatrix[2][2]);
    }

    glm::vec3 Transform::GetRight()
    {
        if (m_Dirty)
            UpdateTransformMatrix();

        return (glm::vec3(m_TransformMatrix[0][0], m_TransformMatrix[0][1], m_TransformMatrix[0][2]));
    }

    glm::vec3 Transform::GetUp()
    {
        if (m_Dirty)
            UpdateTransformMatrix();

        return (glm::vec3(m_TransformMatrix[1][0], m_TransformMatrix[1][1], m_TransformMatrix[1][2]));
    }

    const glm::mat4 &Transform::GetTransformMatrix()
    {
        if (m_Dirty)
            UpdateTransformMatrix();

        return m_TransformMatrix;
    }

    void Transform::UpdateTransformMatrix()
    {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, m_Position);
        matrix = matrix * glm::mat4_cast(glm::quat(glm::radians(m_Rotation)));
        matrix = glm::scale(matrix, m_Scale);
        m_TransformMatrix = matrix;

        m_Dirty = false;
    }
}