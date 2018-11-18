#include "scenegraph.hpp"

SceneGraph::SceneGraph(): 
    name_{},
    root_{} {}

SceneGraph::SceneGraph(std::string name, std::shared_ptr<Node> root):
    name_{name},
    root_{root} {
        std::cout<<"scenegrap created\n";
    }

std::string SceneGraph::getName() const {
    return name_;
}

std::shared_ptr<Node> SceneGraph::getRoot() const {
    return root_;
}


void SceneGraph::setName(std::string name) {
    name_ = name;
} 

void SceneGraph::setRoot (std::shared_ptr<Node> root) {
    root_ = root;
}