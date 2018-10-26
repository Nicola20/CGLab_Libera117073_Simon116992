#include "node.hpp"

class SceneGraph {
    public:
        std::string getName();
        Node getRoot();
        //std::string printGraph();
    private:
        void setName(std::string name);
        void setRoot (Node root);

        std::string name;
        Node root;

};