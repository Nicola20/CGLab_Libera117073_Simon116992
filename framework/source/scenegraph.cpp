#include "scenegraph.hpp"

std::string SceneGraph::getName() {
    return name;
}

Node SceneGraph::getRoot() {
    return root;
}

//std::string printGraph();

void SceneGraph::setName(std::string name) {
    name = name;
} 

void SceneGraph::setRoot (Node root) {
    root = root;
}