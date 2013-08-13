#ifndef SOLID_H
#define SOLID_H

#include <QVector>
#include "face.h"

class Solid3D : public GeomMetaHandle
{
public:
    enum { MAX_VERTICES = 256 };

    Solid3D();

private:
    bool canAddVertex() { return m_Vertices.size() < 256; }

    QVector<Vertex3D>   m_Vertices;
    QVector<Edge3D>     m_Edges;
    QVector<Face3D>     m_Faces;
};

#endif // SOLID_H
