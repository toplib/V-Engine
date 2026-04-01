#include "HierarchyNode.h"

#include <iostream>

namespace Core {
    HierarchyNode::HierarchyNode(HierarchyNode *parent) : parent(parent) {
        if (parent) {
            parent->children.insert(this);
        }
    }

    HierarchyNode::~HierarchyNode() {
        if (parent) {
            parent->removeChild(this);
        }
        for (auto child : children) {
            delete child;
        }
    }

    Transform::Transform* HierarchyNode::getTransform() {
        return &transform;
    }

    glm::mat4 HierarchyNode::getWorldMatrix() {
        if (dirty) updateWorldMatrix();
        return worldMatrix;
    }

    void HierarchyNode::updateWorldMatrix() {
        if (parent->dirty) {
            parent->updateWorldMatrix();
        }

        glm::mat4 local = getTransform()->getModelMatrix();

        worldMatrix = local * parent->getWorldMatrix();

        for (auto child : children) {
            child->updateWorldMatrix();
        }
    }

    void HierarchyNode::addChild(HierarchyNode *child) {
        if (child) {
            this->children.insert(child);
        }
    }

    void HierarchyNode::removeChild(HierarchyNode *child) {
        if (child) {
            children.erase(child);
        }
    }
}
