#include "node.hpp"

       // Node::Node(): {}

    Node* Node::getParent() {
        return parent;
    }

    void Node::setParent(Node* parent) { //I'm not sure about this either I'm using same pointer or same value of pointer
        parent = parent;
    }

    Node* Node::getChildren(std::string child) { //spezielles child aus liste
        return ;
    }

    std::string Node::getName() {
        return name;
    }

    std::string Node::getPath() {
        return path;
    }

    int Node::getDepth() {
        return depth;
    }

   glm::mat4 Node::getLocalTransform() {
        return localTransform;
    }

    void Node::setLocalTransform(glm::mat4 local) {
        localTransform = local;
    }

    glm::mat4 Node::getWorldTransform() {
        return worldTransform;
    }

    void Node::setWorldTransform(glm::mat4 world) {
        worldTransform = world;
    }

    void Node::addChildren(Node* child) {
        children.push_back(child);
    }

    void Node::removeChildren(std::string child) { //get the node with the name child an then remove it; have to traverse through the whole tree
        children.remove(child->name);
    }

    /*
    traversing through a tree:
    start at root node
    if list of children is != empty
    get first child -> do stuff
    get firstchilds chid do the same until list of children is empty;
    get to parent look for second child
    do this until there are no more children
    */
