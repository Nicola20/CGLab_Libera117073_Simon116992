#include "geometrynode.hpp"

GeometryNode::GeometryNode(std::string name, Node* parent, glm::mat4 localt, model geo):
    Node::Node(name, parent,localt),
    geometry_{geo} {
        std::cout<<"created GeometryNode\n";
    }

GeometryNode::GeometryNode(std::string name, float diameter, float rotation_speed, float distance):
    Node::Node(name),
    diameter_{diameter},
    rotation_speed_{rotation_speed},
    distanceToOrigin_{distance}
    //planet_{planet} 
    {}

model GeometryNode::getGeometry() {
    return geometry_;
}

void GeometryNode::setGeometry(model const& geo) {
    geometry_ = geo;
}

planet GeometryNode::getPlanet() {
    return planet_;
}