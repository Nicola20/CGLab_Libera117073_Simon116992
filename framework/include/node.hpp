#include <string>
#include <list>
#include <glm/mat4x4.hpp>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

class Node {
    public:
        //Node();
        Node* getParent();
        void setParent(Node* parent);
        Node getChildren(std::string child);
        std::string getName();
        std::string getPath();
        int getDepth();
        glm::mat4 getLocalTransform();
        void setLocalTransform(glm::mat4 local);
        glm::mat4 getWorldTransform();
        void setWorldTransform(glm::mat4 local);
        void addChildren(Node* child);
        void removeChildren(std::string child);

    private:
        Node* parent;
        std::list<Node*> children;
        std::string name;
        std::string path;
        int depth;
        glm::mat4 localTransform;
        glm::mat4 worldTransform;

}; 
