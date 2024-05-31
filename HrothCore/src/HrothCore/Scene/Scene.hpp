#pragma once

#include <entt.hpp>
#include "HrothCore/Types/Camera.hpp"

namespace HrothCore
{
    class GameObject;
    class UUID;

    class Scene
    {
        public:
            Scene();
            ~Scene();

            GameObject *Instantiate(GameObject *parent);
            GameObject *Instantiate(const std::string &name = "GameObject", GameObject *parent = nullptr);
            void Destroy(GameObject *object);
            GameObject *Duplicate(GameObject *object);

            GameObject *Find(const std::string &name);
            GameObject *Find(const UUID &uuid);

            GameObject *GetRoot() { return m_Root; }
            GameObject *GetSelectedEntity() { return m_SelectedEntity; }
            void SetSelectedEntity(GameObject *object) { m_SelectedEntity = object; }

            void Render(Camera &camera);

        private:
            void UpdateDirtyTransforms(GameObject *object, glm::mat4 parentTransform = glm::mat4(1.0f), bool parentDirty = false);

            entt::registry m_Registry;

            GameObject *m_Root = nullptr;
            std::unordered_map<entt::entity, GameObject *> m_ObjectMap;
            GameObject *m_SelectedEntity = nullptr;

        friend class GameObject;
    };
}