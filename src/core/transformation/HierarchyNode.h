#pragma once
#include <unordered_set>
#include "core/Transform.h"

namespace Core {
    class HierarchyNode {
    public:
        HierarchyNode(HierarchyNode* parent = nullptr);
        ~HierarchyNode();

        Transform::Transform* getTransform();

        glm::mat4 getWorldMatrix();
        void updateWorldMatrix();

        void addChild(HierarchyNode* child);
        void removeChild(HierarchyNode* child);
    private:
        bool dirty = true;
        Transform::Transform transform;
        std::unordered_set<HierarchyNode*> children;
        HierarchyNode* parent;
        glm::mat4 worldMatrix;
    };
}
