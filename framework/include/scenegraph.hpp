#include "node.hpp"
#include <iostream>

class SceneGraph {
    public:
    SceneGraph();
        SceneGraph(std::string name, std::shared_ptr<Node> root);
        std::string getName() const;
        std::shared_ptr<Node> getRoot() const;
        //std::string printGraph() const;
        void setName(std::string name);
        void setRoot (std::shared_ptr<Node> root);

    private:
        std::string name_;
        std::shared_ptr<Node> root_;

};