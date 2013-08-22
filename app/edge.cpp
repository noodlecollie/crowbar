#include "edge.h"
#include "solid.h"

bool Edge3D::calcMidpoint(Solid3D &parent)
{
    Vertex3D* start = parent.findVertex(getStartVertex());
    if ( !start )
    {
        setMidpoint(VEC3_ORIGIN);
        return false;
    }
    
    Vertex3D* end = parent.findVertex(getEndVertex());
    if ( !end )
    {
        setMidpoint(VEC3_ORIGIN);
        return false;
    }
    
    setMidpoint( start->getPosition() + ((end->getPosition() - start->getPosition())/2.0) );
    return true;
}
