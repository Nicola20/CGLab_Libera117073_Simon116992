#include "node.hpp"
#include <iostream>

class SceneGraph {
    public:
    SceneGraph();
        SceneGraph(std::string name, Node* root);
        std::string getName() const;
        Node* getRoot() const;
        //std::string printGraph() const;
    private:
        void setName(std::string name);
        void setRoot (Node* root);

        std::string name_;
        Node* root_;

};