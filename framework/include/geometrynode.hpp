#include "model.hpp"
#include "node.hpp"
#include "structs.hpp"

class GeometryNode: public Node {

    public:
        GeometryNode(std::string name, Node* parent, glm::mat4 localt, model geo);
        GeometryNode(std::string name, float diameter, float rotation_speed, float distance);
        model getGeometry();
        void setGeometry(model const& geo); 
        planet getPlanet();

    private:
        model geometry_;
        //planet planet_;
        float diameter_;
        float rotation_speed_;
        float distanceToOrigin_;

};
