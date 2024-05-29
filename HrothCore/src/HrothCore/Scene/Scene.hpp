#pragma once

#include <entt.hpp>
#include <uuid_v4.h>

namespace HrothCore
{
    class GameObject;

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
            GameObject *Find(const UUIDv4::UUID &uuid);

            GameObject *GetRoot() { return m_Root; }

        private:
            entt::registry m_Registry;

            GameObject *m_Root = nullptr;
            std::unordered_map<entt::entity, GameObject *> m_ObjectMap;

        friend class GameObject;
    };
}