#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Scene.hpp"

#include <fstream>

namespace HrothCore
{
    Scene::Scene()
    {
        // root node
        Nodes.push_back(SceneNode{
            .Position = 0,
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
            if (Nodes[i].IsMarkForDeletion)
                continue;
            std::string name = "";
            if (Names.contains(i))
            {
                name = Names.at(i);
            }

            file << "n" << i << " [label=\"" << name << "\"]\n";
        }
        for (int i = 0; i < Nodes.size(); i++)
        {
            if (Nodes[i].IsMarkForDeletion)
                continue;
            int p = Nodes[i].Parent;
            if (p > -1)
                file << "\t n" << p << " -> n" << i << "\n";
        }
        file << "}\n";
        file.close();
    }

    NodeView Scene::AddNode()
    {
        return AddNode(NodeView(Nodes[0]));
    }

    NodeView Scene::AddNode(NodeView parentView)
    {
        HC_ASSERT(parent.Position < Nodes.size());

        SceneNode &parent = parentView.Node;

        SceneNode node;
        node.Position = (int)Nodes.size();
        node.Parent = parent.Position;
        node.Level = parent.Level + 1;
        node.NextSibling = -1;
        node.FirstChild  = -1;
        node.LastSibling = -1;

        // refresh siblings hierarchy
        int child = parent.FirstChild;
        if (child == -1)
        {
            parent.FirstChild = node.Position;
            node.LastSibling = node.Position;
        }
        else
        {
            for (; Nodes[child].NextSibling != -1; child = Nodes[child].NextSibling);
            {
                Nodes[child].LastSibling = node.Position;
            }
            Nodes[child].NextSibling = node.Position;
            Nodes[child].LastSibling = node.Position;
        }

        Nodes.push_back(node);
        LocalTransforms.push_back(glm::mat4(1.0f));
        GlobalTransforms.push_back(glm::mat4(1.0f));

        return Nodes[node.Position];
    }

    void Scene::RemoveNode(NodeView nodeView)
    {
        SceneNode &node = nodeView.Node;

        node.IsMarkForDeletion = true;

        for (int child = node.FirstChild; child != -1; child = Nodes[child].NextSibling)
            RemoveNode(Nodes[child]);
    }

    NodeView Scene::FindNode(const std::string &name)
    {
        return Nodes[0];
    }

    void Scene::RecalculateGlobalTransforms()
    {
    }

    void Scene::MarkNodeTransformDirty(int node)
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

    void Scene::SetNodeName(NodeView nodeView, const std::string &name)
    {
        Names[nodeView.Node.Position] = name;
    }
}