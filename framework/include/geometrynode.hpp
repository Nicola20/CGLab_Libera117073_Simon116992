#include "model.hpp"
#include "node.hpp"
#include "structs.hpp"

class GeometryNode: public Node {

    public:
        GeometryNode(std::string name, Node* parent, glm::mat4 localt, model geo);
        GeometryNode(std::string name, float diameter, float rotation_speed, float distance);
        model getGeometry() const;
        void setGeometry(model const& geo); 
        //planet getPlanet();
        float getDiameter()const;
        float getRotation() const;
        float getDistance() const;

    private:
        model geometry_;
        //planet planet_;
        //used extra parameters for describing planets because a struct didn't work quite well for now
        float diameter_;
        float rotation_speed_;
        float distanceToOrigin_;

};
