#include "geometrynode.hpp"

model GeometryNode::getGeometry() {
    return geometry;
}

void GeometryNode::setGeometry(model const& geo) {
    geometry = geo;
}