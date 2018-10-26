#include "node.hpp"

class CameraNode: public Node {
    public:
        bool getPersepective();
        bool getEnabled();
        void setEnabled(bool en);
        glm::mat4 getProjectionMatrix();
        void setProjectionMatrix(glm::mat4 mat);
    
    private:
        bool isPerspective;
        bool isEnabled;
        glm::mat4 projectionMatrix;
};