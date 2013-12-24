#ifndef EDGE3D_H
#define EDGE3D_H

#include <QVector3D>

#include "geommetahandle.h"
#include "vertex3d.h"
#include <QPair>

class Vertex3D;

namespace Geometry
{
    // Edge has no implicit orientation (ie. start/end vertices) because it could be used in either direction
    // by adjacent polygons.
    class Edge3D : public GeomMetaHandle
    {
        Q_PROPERTY(Vertex3D* vertexA READ vertexA WRITE setVertexA)
        Q_PROPERTY(Vertex3D* vertexB READ vertexB WRITE setVertexB)
    public:
        Edge3D(QObject *parent = 0);
        Edge3D(Vertex3D *vertA, Vertex3D *vertB, QObject *parent = 0);
        Edge3D(const QPair<Vertex3D*, Vertex3D*> &pair, QObject *parent = 0);
        
        QVector3D midpoint(bool* success) const;
        bool hasMidpoint() const;
        virtual bool isValid() const;
        
        void setVertexA(Vertex3D* vertA);
        Vertex3D* vertexA() const;
        
        void setVertexB(Vertex3D* vertB);
        Vertex3D* vertexB() const;
        
        void setVertex(const QPair<Vertex3D*, Vertex3D*> &pair);
        QPair<Vertex3D*, Vertex3D*> getVertex() const;
        
        // Returns 0 if the given vertex is A, 1 if B, -1 otherwise.
        int containsVertex(Vertex3D* vert);
        
        // n == 0 or 1
        void setVertex(int n, Vertex3D* vert);
        Vertex3D* getVertex(int n) const;
        
    private:
        Vertex3D* m_pVertexA;
        Vertex3D* m_pVertexB;
    };
}

#endif // EDGE3D_H
