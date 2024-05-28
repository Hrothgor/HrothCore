#pragma once

#include "HrothCore/Core/AssetManager.hpp"
#include "HrothCore/Types/GameObject.hpp"

namespace HrothCore
{
    constexpr const int MAX_NODE_LEVEL = 32;

    struct SceneNode
    {
        GameObject Object;

        int Position = 0;
        int Parent = 0;
        int FirstChild = -1;
        int NextSibling = -1;
        int LastSibling = -1;
        int Level = 0;

        bool IsMarkForDeletion = false;
    };

    struct NodeView
    {
        public:
            GameObject &GetGameObject() { return Node.Object; }
        private:
            SceneNode &Node;
            NodeView(SceneNode &node) : Node(node) {}

        friend class Scene;
    };

    class Scene
    {
        public:
            Scene();
            ~Scene();
            
            void Load(const std::string &path);
            void Copy(const Scene &scene);
            void Save(const std::string &path) const;

            void DumpToDot(const std::string &path) const;

            NodeView AddNode();
            NodeView AddNode(NodeView parent);
            void RemoveNode(NodeView nodeView);

            NodeView FindNode(const std::string &name);
            NodeView GetParentNode(NodeView nodeView) const;
            std::vector<NodeView> GetChildNodes(NodeView nodeView) const;

            // Matrixes (maybe component ?)
            void RecalculateGlobalTransforms();

            // Meshes (component)
            AssetRef<Mesh> GetNodeMesh(int node) const;
            void SetNodeMesh(int node, const AssetRef<Mesh> &mesh);
            // Name (component): Which name is assigned to the node
            int GetNodeName(int node) const;
            void SetNodeName(NodeView nodeView, const std::string &name);

        private:
            void MarkNodeTransformDirty(int node);

            std::vector<SceneNode> Nodes;

            std::vector<int> DirtyNodes[MAX_NODE_LEVEL];
            // Matrixes (maybe component ?)
            std::vector<glm::mat4> LocalTransforms;
            std::vector<glm::mat4> GlobalTransforms;

            // Meshes (component)
            std::unordered_map<int, AssetRef<Mesh>> Meshes;
            // Name (component): Which name is assigned to the node
	        std::unordered_map<int, std::string> Names;
    };
}