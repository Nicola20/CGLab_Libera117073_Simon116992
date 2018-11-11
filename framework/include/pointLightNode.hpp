#ifndef POINTLIGHTNODE_HPP
#define POINTLIGHTNODE_HPP

#include "node.hpp"

class PointLightNode: public Node {
    public:
    float getLightIntensity() const;
    void setLightIntensity(float intense);

    glm::vec3 getColor() const;
    void setColor (glm::vec3 col);

    private:
    float lightIntensity_;
    glm::vec3 color_;

};
#endif