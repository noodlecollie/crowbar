#include <Qt3D/QVector3DArray>

#include "renderbox.h"
#include "geomutil.h"

GEOMETRY_BEGIN_NAMESPACE

RenderBox::RenderBox() :
    QBox3D(), m_vecPosition(VEC3_ORIGIN), m_colColour(255,255,255,255)
{
}

RenderBox::RenderBox(const QVector3D &origin, const QVector3D &min, const QVector3D &max) :
    QBox3D(min, max), m_vecPosition(origin), m_colColour(255,255,255,255)
{
}

QColor RenderBox::color() const
{
    return m_colColour;
}

void RenderBox::setColor(const QColor &col)
{
    m_colColour = col;
}

QVector3D RenderBox::wOrigin() const
{
    return m_vecPosition;
}

void RenderBox::setWOrigin(const QVector3D &pos)
{
    m_vecPosition = pos;
}

QVector3D RenderBox::wCenter() const
{
    return m_vecPosition + center();
}

QGeometryData RenderBox::toGeomData() const
{
    // Calculate the min and max co-ordinates in world space.
    QVector3D min = m_vecPosition + minimum();
    QVector3D max = m_vecPosition + maximum();
    QColor col = color();
    
    // Order of vertices:
    // 0. Minimum
    // 1. Minimum with modified X co-ord
    // 2. Minimum with modified X and Z co-ords
    // 3. Minimum with modified Z co-ord
    // 4. Maximum with modified Z co-ord
    // 5. Maximum with modified Z and X co-ords
    // 6. Maximum with modified X co-ord
    // 7. Maximum
    // This gives us an easy counter-clockwise winding for the top and bottom faces.
    
    const QVector3D verts[8] =               // Taking Z as "north" and looking down on the box (Y pointing at us), we have:
    {
        min,                                    // Lower SW 0
        QVector3D(max.x(), min.y(), min.z()),   // Lower NW 1
        QVector3D(max.x(), min.y(), max.z()),   // Lower NE 2
        QVector3D(min.x(), min.y(), max.z()),   // Lower SE 3
        QVector3D(max.x(), max.y(), min.z()),   // Upper NW 4
        QVector3D(min.x(), max.y(), min.z()),   // Upper SW 5
        QVector3D(min.x(), max.y(), max.z()),   // Upper SE 6
        max                                     // Upper NE 7
    };
    
    // Normals for faces
    const QVector3D nTop(0, 1, 0);      // Up    = +Y
    const QVector3D nBottom(0, -1, 0);  // Down  = -Y
    const QVector3D nNorth(0, 0, 1);    // North = +Z
    const QVector3D nSouth(0, 0, -1);   // South = -Z
    const QVector3D nEast(-1, 0, 0);    // East  = -X
    const QVector3D nWest(1, 0, 0);     // West  = +X
    
    // Create a GeomData and add each face of the box.
    QGeometryData geom;
    
    // Top face
    geom.appendVertex(verts[4], verts[5], verts[6], verts[7]);
    geom.appendNormal(nTop, nTop, nTop, nTop);
    
    // Bottom face
    geom.appendVertex(verts[0], verts[1], verts[2], verts[3]);
    geom.appendNormal(nBottom, nBottom, nBottom, nBottom);
    
    // North face
    geom.appendVertex(verts[7], verts[2], verts[1], verts[4]);
    geom.appendNormal(nNorth, nNorth, nNorth, nNorth);
    
    // South face
    geom.appendVertex(verts[0], verts[3], verts[6], verts[5]);
    geom.appendNormal(nSouth, nSouth, nSouth, nSouth);
    
    // East face
    geom.appendVertex(verts[7], verts[6], verts[3], verts[2]);
    geom.appendNormal(nEast, nEast, nEast, nEast);
    
    // West face
    geom.appendVertex(verts[0], verts[5], verts[4], verts[1]);
    geom.appendNormal(nWest, nWest, nWest, nWest);
    
    // Append colour attributes.
    // These will all be the same.
    for ( int i = 0; i < 24; i++ )
    {
        geom.appendColor(col);
    }
    
    // Return the GeomData.
    return geom;
}

GEOMETRY_END_NAMESPACE
