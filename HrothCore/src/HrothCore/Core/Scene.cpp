#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Scene.hpp"

#include <fstream>

namespace HrothCore
{
    Scene::Scene()
    {
        // root node
        Nodes.push_back(SceneNode{
            .Parent = -1,
            .FirstChild = -1,
            .NextSibling = -1,
            .LastSibling = -1,
            .Level = 0
        });

        LocalTransforms.push_back(glm::mat4(1.0f));
        GlobalTransforms.push_back(glm::mat4(1.0f));
    }

    Scene::~Scene()
    {
    }

    void Scene::Load(const std::string &path)
    {
    }

    void Scene::Copy(const Scene &scene)
    {
    }

    void Scene::Save(const std::string &path) const
    {
    }

    void Scene::DumpToDot(const std::string &path) const
    {
        std::ofstream file(path, std::ofstream::out | std::ofstream::trunc);

        file << "digraph G\n{\n";
        for (int i = 0; i < Nodes.size(); i++)
        {
            std::string name = "";
            if (Names.contains(i))
            {
                name = Names.at(i);
            }

            file << "n" << i << " [label=\"" << name << "\"]\n";
        }
        for (int i = 0; i < Nodes.size(); i++)
        {
            int p = Nodes[i].Parent;
            if (p > -1)
                file << "\t n" << p << " -> n" << i << "\n";
        }
        file << "}\n";
        file.close();
    }

    int Scene::AddNode(int parent)
    {
        HC_ASSERT(parent < Nodes.size());

        int node = (int)Nodes.size();
        {
            // TODO: resize aux arrays (local/global etc.)
            LocalTransforms.push_back(glm::mat4(1.0f));
            GlobalTransforms.push_back(glm::mat4(1.0f));
        }

        Nodes.push_back(SceneNode{ .Parent = parent });

        // find first item (sibling)
        int child = Nodes[parent].FirstChild;
        if (child == -1)
        {
            Nodes[parent].FirstChild = node;
            Nodes[node].LastSibling = node;
        }
        else
        {
            for (; Nodes[child].NextSibling != -1; child = Nodes[child].NextSibling);
            {
                Nodes[child].LastSibling = node;
            }
            Nodes[child].NextSibling = node;
            Nodes[child].LastSibling = node;
        }
        Nodes[node].Level = Nodes[parent].Level + 1;
        Nodes[node].NextSibling = -1;
        Nodes[node].FirstChild  = -1;
        return node;
    }

    void Scene::RemoveNode(int node)
    {
    }

    int Scene::FindNode(const std::string &name)
    {
        return 0;
    }

    void Scene::RecalculateGlobalTransforms()
    {
    }

    AssetRef<Mesh> Scene::GetNodeMesh(int node) const
    {
        return AssetRef<Mesh>(0);
    }

    void Scene::SetNodeMesh(int node, const AssetRef<Mesh> &mesh)
    {
    }

    int Scene::GetNodeName(int node) const
    {
        return 0;
    }

    void Scene::SetNodeName(int node, const std::string &name)
    {
        Names[node] = name;
    }

    void Scene::MarkNodeDirty(int node)
    {
    }
}