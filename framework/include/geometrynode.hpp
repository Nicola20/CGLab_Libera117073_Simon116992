#include "model.hpp"
#include "node.hpp"

class GeometryNode: public Node {

    public:
        model getGeometry();
        void setGeometry(model geo); 

    private:
        model geometry;

}
