#include "pointLightNode.hpp"


    PointLightNode::PointLightNode() {}

    PointLightNode::PointLightNode(float intensity, glm::vec3 col):
        lightIntensity_{intensity},
        color_{col} {}


    float PointLightNode::getLightIntensity() const {
        return lightIntensity_;
    }


    void PointLightNode::setLightIntensity(float intense) {
        lightIntensity_ = intense;
    }

    glm::vec3 PointLightNode::getColor() const {
        return color_;
    }


    void PointLightNode::setColor (glm::vec3 const& col) {
        color_ = col;
    }
   
   /*
    float PointLightNode::getPosition() const {
        return position_;
    }*/


