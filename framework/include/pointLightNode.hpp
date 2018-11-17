#ifndef POINTLIGHTNODE_HPP
#define POINTLIGHTNODE_HPP

#include "node.hpp"

class PointLightNode: public Node {
    public:
    PointLightNode();
    PointLightNode(float intensity, glm::vec3 col);
    float getLightIntensity() const;
    void setLightIntensity(float intense);

    glm::vec3 getColor() const;
    void setColor (glm::vec3 const& col);

    //float getPosition() const;

    private:
    float lightIntensity_;
    glm::vec3 color_;
    //float position_;

};
#endif