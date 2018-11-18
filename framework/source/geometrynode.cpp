#include "geometrynode.hpp"
/*
GeometryNode::GeometryNode(std::string name, glm::mat4 localt, model_object geo):
    Node::Node(name, localt),
    geometry_{geo} {
        std::cout<<"created GeometryNode\n";
    }*/

GeometryNode::GeometryNode(std::string name, int depth, float diameter, float rotation_speed, float distance, float selfRotation):
    Node::Node(name, depth, diameter, rotation_speed, distance, selfRotation)  {}
    //planet_{planet} 
 

model_object GeometryNode::getGeometry() const{
    return geometry_;
}

void GeometryNode::setGeometry(model_object const& geo) {
    geometry_ = geo;
}
/*
float GeometryNode::getDiameter()const {
    return diameter_;
}

float GeometryNode::getRotation() const {
    return rotation_speed_;
}

float GeometryNode::getDistance() const {
    return distanceToOrigin_;
}*/

/*
planet GeometryNode::getPlanet() {
    return planet_;
}*/