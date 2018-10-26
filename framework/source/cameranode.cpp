#include "cameranode.hpp"

    bool CameraNode::getPersepective() {
        return isPerspective;
    }

    bool CameraNode::getEnabled() {
        return isEnabled;
    }

    void CameraNode::setEnabled(bool en) {
        isEnabled = en;
    }

    glm::mat4 CameraNode::getProjectionMatrix() {
        return projectionMatrix;
    }

    void CameraNode::setProjectionMatrix(glm::mat4 mat) {
        projectionMatrix = mat;
    }