#include "HrothCore_pch.hpp"

#include "HrothCore/Scene/GameObject.hpp"

namespace HrothCore
{
    GameObject::GameObject(entt::entity handle, Scene *scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }

    GameObject::~GameObject()
    {
    }

    void GameObject::AttachToParent(GameObject *parent)
    {
        if (!parent || IsParentOf(parent) || parent == m_Parent || parent == this)
            return;

        if (m_Parent)
            m_Parent->RemoveChild(this);

        m_Parent = parent;
        parent->AddChild(this);
    }

    void GameObject::DetachFromParent()
    {
        AttachToParent(m_Scene->GetRoot());
    }

    void GameObject::AddChild(GameObject *child)
    {
        if (!child)
            return;

        m_Children.push_back(child);
    }

    void GameObject::RemoveChild(GameObject *child)
    {
        if (!child)
            return;

        auto it = std::find(m_Children.begin(), m_Children.end(), child);
        if (it != m_Children.end())
            m_Children.erase(it);
    }

    bool GameObject::IsParentOf(GameObject *child)
    {
        if (!child)
            return false;

        auto it = std::find(m_Children.begin(), m_Children.end(), child);
        if (it != m_Children.end())
            return true;

        for (auto c : m_Children)
            if (c->IsParentOf(child))
                return true;

        return false;
    }
}