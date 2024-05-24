#pragma once

#include "HrothCore/Core/AssetManager.hpp"


namespace HrothCore
{
    constexpr const int MAX_NODE_LEVEL = 32;

    struct SceneNode
    {
        int Parent = 0;
        int FirstChild = -1;
        int NextSibling = -1;
        int LastSibling = -1;
        int Level = 0;
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

            int AddNode(int parent = 0);
            void RemoveNode(int node);

            int FindNode(const std::string &name);
            int GetNodeLevel(int node) const;
            int GetParentNode(int node) const;

            // Matrixes (maybe component ?)
            void RecalculateGlobalTransforms();

            // Meshes (component)
            AssetRef<Mesh> GetNodeMesh(int node) const;
            void SetNodeMesh(int node, const AssetRef<Mesh> &mesh);
            // Name (component): Which name is assigned to the node
            int GetNodeName(int node) const;
            void SetNodeName(int node, const std::string &name);

        private:
            void MarkNodeDirty(int node);

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