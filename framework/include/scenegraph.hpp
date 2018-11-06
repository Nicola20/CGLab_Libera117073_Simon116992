#include "node.hpp"
#include <iostream>

class SceneGraph {
    public:
    SceneGraph();
        SceneGraph(std::string name, Node* root);
        std::string getName() const;
        Node* getRoot() const;
        //std::string printGraph() const;
        void setName(std::string name);
        void setRoot (Node* root);

    private:
        std::string name_;
        Node* root_;

};