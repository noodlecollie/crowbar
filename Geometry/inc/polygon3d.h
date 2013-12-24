#ifndef POLYGON3D_H
#define POLYGON3D_H

#include "geommetahandle.h"
#include "edge3d.h"

namespace Geometry
{
    class Polygon3D : public GeomMetaHandle
    {
    public:
        Polygon3D(QObject *parent = 0);
        
        // Returns whether the face is valid.
        // All edges must be valid and form a closed loop.
        virtual bool isValid() const;
        
    private:
        // Returns whether the edges form a closed loop.
        // Size of array is assumed to be 3.
        bool closedLoop(Edge3D* edges[]);
        
        Edge3D* m_pEdges[3];
    };
}

#endif // POLYGON3D_H
