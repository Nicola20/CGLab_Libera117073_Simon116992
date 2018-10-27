#include "model.hpp"
#include "node.hpp"

class GeometryNode: public Node {

    public:
        model getGeometry();
        void setGeometry(model const& geo); 

    private:
        model geometry;

};
