#include "model.hpp"
#include "node.hpp"
#include "structs.hpp"

class GeometryNode: public Node {

    public:
        //GeometryNode(std::string name, glm::mat4 localt, model_object geo);
        GeometryNode(std::string name, int depth, float diameter, float rotation_speed, float distance, float selfRotation);
        model_object getGeometry() const;
        void setGeometry(model_object const& geo); 
        //planet getPlanet();
        /*
        float getDiameter()const;
        float getRotation() const;
        float getDistance() const;*/

    private:
        model_object geometry_;
        //planet planet_;
        //used extra parameters for describing planets because a struct didn't work quite well for now
       /* 
        float diameter_;
        float rotation_speed_;
        float distanceToOrigin_;
        float selfRotation_; */

};
