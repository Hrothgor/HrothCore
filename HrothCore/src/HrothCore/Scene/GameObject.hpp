#pragma once

#include "HrothCore/Scene/Scene.hpp"
#include <entt.hpp>

namespace HrothCore
{
    struct Hierarchy
    {
        uint32_t children = 0;
        entt::entity first{entt::null}; // first child
        entt::entity prev{entt::null}; // prev sibling
        entt::entity next{entt::null}; // next sibling
        entt::entity parent{entt::null}; // parent
    };

    class GameObject
    {
        public:
            GameObject(entt::entity handle, Scene *scene);
            GameObject(const GameObject& other) = default;
            ~GameObject();

            GameObject *GetParent() const { return m_Parent; }
            std::vector<GameObject *> GetChilds() const { return m_Children; }

            void AttachToParent(GameObject *parent);
            void DetachFromParent();

            template<typename T, typename... Args>
            T& AddComponent(Args&&... args)
            {
                HC_ASSERT(!HasComponent<T>(), "Entity already has component!");
                T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
                return component;
            }

            template<typename T, typename... Args>
            T& AddOrReplaceComponent(Args&&... args)
            {
                T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
                return component;
            }

            template<typename T>
            T& GetComponent()
            {
                HC_ASSERT(HasComponent<T>(), "Entity does not have component!");
                return m_Scene->m_Registry.get<T>(m_EntityHandle);
            }

            template<typename T>
            T* TryGetComponent()
            {
                return m_Scene->m_Registry.try_get<T>(m_EntityHandle);
            }

            template<typename T>
            bool HasComponent()
            {
                return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
            }

            template<typename T>
            void RemoveComponent()
            {
                HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
                m_Scene->m_Registry.remove<T>(m_EntityHandle);
            }

            bool IsValid() const { return m_EntityHandle != entt::null; }

            bool operator==(const GameObject& other) const
            {
                return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
            }

            bool operator!=(const GameObject& other) const
            {
                return !operator==(other);
            }
            
        private:
            void AddChild(GameObject *child);
            void RemoveChild(GameObject *child);
            bool IsParentOf(GameObject *child);

            entt::entity m_EntityHandle{ entt::null };
            Scene *m_Scene = nullptr;

            GameObject *m_Parent = nullptr;
            std::vector<GameObject *> m_Children;
    
        friend class Scene;
    };
}