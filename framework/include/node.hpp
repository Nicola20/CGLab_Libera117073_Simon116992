#ifndef NODE_HPP
#define NODE_HPP

#include "structs.hpp"
#include <string>
#include <list>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <algorithm>
#include <iostream>
#include <memory>

class Node {
    public:
        Node();
        Node(std::string name, std::shared_ptr<Node> parent, glm::mat4 localtransform);
        Node(std::string name);
        Node (std::string name, int depth);
        Node(std::string name, int depth, float diameter, float rotation_speed, float distance, float selfRotation);
        std::shared_ptr<Node> getParent() const;
        void setParent(std::shared_ptr<Node> parent);
        std::shared_ptr<Node> getChildren(std::string child) const;
        std::string getName() const;
        std::string getPath() const;
        int getDepth() const;
        glm::mat4 getLocalTransform() const;
        void setLocalTransform(glm::mat4 local);
        glm::mat4 getWorldTransform() const;
        void setWorldTransform(glm::mat4 local);
        void addChildren(std::shared_ptr<Node> child);
        std::shared_ptr<Node> removeChildren(std::string child);
        virtual std::list<std::shared_ptr<Node>> getListOfChildren();

        //std::shared_ptr<Node> getFirstChild(std::shared_ptr<Node> i) const;

        float getDiameter()const;
        float getRotation() const;
        float getDistance() const;
        glm::vec3 getPlanetColor() const;
        void setPlanetColor(glm::vec3 const& col);
        float getSelfRotation() const;
        void setPlanetTex(texture_object const& tex);
        texture_object getPlanetTex() const;

    private:
       std::shared_ptr<Node> parent_;
        std::list<std::shared_ptr<Node>> children_;
        std::string name_;
        std::string path_;
        int depth_;
        glm::mat4 localTransform_;
        glm::mat4 worldTransform_;

        //settings for the planet
        float diameter_; 
        float rotation_speed_;
        float distanceToOrigin_;
        float selfRotation_;
        glm::vec3 planetCol_;
        //texture for planet
        texture_object planetTex_;

}; 
#endif
