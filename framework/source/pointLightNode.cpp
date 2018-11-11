#include "pointLightNode.hpp"

    float PointLightNode::getLightIntensity() const {
        return lightIntensity_;
    }


    void PointLightNode::setLightIntensity(float intense) {
        lightIntensity_ = intense;
    }

    glm::vec3 PointLightNode::getColor() const {
        return color_;
    }


    void PointLightNode::setColor (glm::vec3 col) {
        color_ = col;
    }


