#include "node.hpp"

    Node::Node() {}

    Node::Node(std::string name, std::shared_ptr<Node> parent, glm::mat4 localtransform):
        name_{name},
        parent_{parent},
        localTransform_{localtransform}{
            std::cout<<"Node created \n";
        }

    Node::Node(std::string name, int depth, float diameter, float rotation_speed, float distance, float selfRotation):
        name_{name},
        depth_{depth},
        diameter_{diameter},
        rotation_speed_{rotation_speed},
        distanceToOrigin_{distance},
        selfRotation_{selfRotation} {}

    Node::Node(std::string name):
    name_{name} {}

    Node::Node (std::string name, int depth):
        name_{name},
        depth_{depth} {}

    std::shared_ptr<Node> Node::getParent() const {
        return parent_;
    }

    void Node::setParent(std::shared_ptr<Node> parent) { //I'm not sure about this either I'm using same pointer or same value of pointert
        parent_ = parent;
    }

    std::shared_ptr<Node> Node::getChildren(std::string child) const { //spezielles child aus liste
        auto i = std::find_if(children_.begin(), children_.end(),[&child](std::shared_ptr<Node> node) {return (node->getName() == child);});
            if (i != children_.end()) {
                return *i;
            } else {
                return nullptr;
            }
    }

    std::string Node::getName() const {
        return name_;
    }

    std::string Node::getPath() const {
        return path_;
    }

    int Node::getDepth() const {
        return depth_;
    }

   glm::mat4 Node::getLocalTransform() const {
        return localTransform_;
    }

    void Node::setLocalTransform(glm::mat4 local) {
        localTransform_ = local;
    }

    glm::mat4 Node::getWorldTransform() const {
        return worldTransform_;
    }

    void Node::setWorldTransform(glm::mat4 world) {
        worldTransform_ = world;
    }

    void Node::addChildren(std::shared_ptr<Node> child) {
        children_.push_back(child);
    }

    std::shared_ptr<Node> Node::removeChildren(std::string child) { 
        auto i = std::find_if(children_.begin(), children_.end(),[&child](std::shared_ptr<Node> node) {return (node->getName() == child);});
            if(i != children_.end()) {
                children_.remove(*i);
                return *i;
            } else {
                return nullptr;
            }
    }

    std::list<std::shared_ptr<Node>> Node::getListOfChildren() {
        return children_;
    }

float Node::getDiameter()const {
    return diameter_;
}

float Node::getRotation() const {
    return rotation_speed_;
}

float Node::getDistance() const {
    return distanceToOrigin_;
}

glm::vec3 Node::getPlanetColor() const {
    return planetCol_;
}

float Node::getSelfRotation() const {
    return selfRotation_;
}

void Node::setPlanetColor(glm::vec3 const& col) {
    planetCol_ = col;
}
/*
std::shared_ptr<Node> Node::getFirstChild(std::shared_ptr<Node> i) const {
    auto list = i->getListOfChildren();
    auto it = list.begin();
    return *it;
}*/
