#include "HrothCore_pch.hpp"

#include "HrothCore/Scene/Scene.hpp"
#include "HrothCore/Scene/GameObject.hpp"

#include "HrothCore/Components/IDComponent.hpp"
#include "HrothCore/Components/TransformComponent.hpp"

#include "HrothCore/Utils/UUID.hpp"

#include "HrothCore/Renderer/Renderer.hpp"

#include <fstream>

namespace HrothCore
{
    Scene::Scene()
    {
        m_Root = new GameObject(m_Registry.create(), this);
        m_Root->AddComponent<IDComponent>("Root");
        m_Root->AddComponent<TransformComponent>();
    }

    Scene::~Scene()
    {
        delete m_Root;
        for (auto object : m_ObjectMap)
            delete object.second;
    }

    GameObject *Scene::Instantiate(GameObject *parent)
    {
        return Instantiate("GameObject", parent);
    }

    GameObject *Scene::Instantiate(const std::string &name, GameObject *parent)
    {
        entt::entity entity = m_Registry.create();
        GameObject *object = new GameObject(entity, this);
        object->AddComponent<IDComponent>(name);
        object->AddComponent<TransformComponent>();

        object->AttachToParent(parent ? parent : m_Root);

        m_ObjectMap[entity] = object;

        return object;
    }

    void Scene::Destroy(GameObject *object)
    {
        m_Registry.destroy(object->m_EntityHandle);

        for (auto child : object->GetChilds())
            Destroy(child);

        m_ObjectMap.erase(object->m_EntityHandle);
        delete object;
    }

    GameObject *Scene::Duplicate(GameObject *object)
    {
        return nullptr; // TODO
    }

    GameObject *Scene::Find(const std::string &name)
    {
        auto view = m_Registry.view<IDComponent>();

        for (auto entity : view)
        {
            auto &id = view.get<IDComponent>(entity);
            if (id.Name == name)
                return m_ObjectMap[entity];
        }

        return nullptr;
    }

    GameObject *Scene::Find(const UUID &uuid)
    {
        auto view = m_Registry.view<IDComponent>();

        for (auto entity : view)
        {
            auto &id = view.get<IDComponent>(entity);
            if (id.Uuid == uuid)
                return m_ObjectMap[entity];
        }

        return nullptr;
    }

    void Scene::UpdateDirtyTransforms(GameObject *object, glm::mat4 parentTransform, bool parentDirty)
    {
        auto &transform = object->GetComponent<TransformComponent>();
        if (transform.IsDirty || parentDirty)
        {
            transform.UpdateLocal();
            transform.Global = parentTransform * transform.Local;
            transform.IsDirty = false;
            for (GameObject *child : object->GetChilds())
                UpdateDirtyTransforms(child, transform.Global, true);
            return;
        }
        for (GameObject *child : object->GetChilds())
                UpdateDirtyTransforms(child, transform.Global, false);
    }

    void Scene::Render(Camera &camera)
    {
        UpdateDirtyTransforms(GetRoot());

        Renderer::BeginScene(camera);
        Renderer::EndScene();
    }
}