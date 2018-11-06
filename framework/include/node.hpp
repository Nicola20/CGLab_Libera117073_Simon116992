#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <list>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <algorithm>
#include <iostream>

class Node {
    public:
        Node(std::string name, Node* parent, glm::mat4 localtransform);
        Node(std::string name);
        Node(std::string name, float diameter, float rotation_speed, float distance);
        Node* getParent() const;
        void setParent(Node* parent);
        Node* getChildren(std::string child) const;
        std::string getName() const;
        std::string getPath() const;
        int getDepth() const;
        glm::mat4 getLocalTransform() const;
        void setLocalTransform(glm::mat4 local);
        glm::mat4 getWorldTransform() const;
        void setWorldTransform(glm::mat4 local);
        void addChildren(Node* child);
        Node* removeChildren(std::string child);
        virtual std::list<Node*> getListOfChildren();

        float getDiameter()const;
        float getRotation() const;
        float getDistance() const;

    private:
        Node* parent_;
        std::list<Node*> children_;
        std::string name_;
        std::string path_;
        int depth_;
        glm::mat4 localTransform_;
        glm::mat4 worldTransform_;

        float diameter_;
        float rotation_speed_;
        float distanceToOrigin_;

}; 
#endif
