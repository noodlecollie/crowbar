#include <Qt3D/QVector3DArray>

#include "renderbox.h"
#include "geomutil.h"

GEOMETRY_BEGIN_NAMESPACE

RenderBox::RenderBox() :
    QBox3D(), m_colColour(255,255,255,255)
{
}

RenderBox::RenderBox(const QVector3D &corner1, const QVector3D &corner2) :
    QBox3D(corner1, corner2), m_colColour(255,255,255,255)
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

QGeometryData RenderBox::toGeomData() const
{
    QVector3D min = minimum();
    QVector3D max = maximum();
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
    // This gives us counter-clockwise winding for the top and bottom faces.
    
    const QVector3D verts[8] =               // Taking Y as "north" and looking down on the box, we have:
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
    
    // Calculate normals for faces.
    QVector3D nTop = ccwNormal(verts[5], verts[6], verts[7]);       // SW, SE, NE
    QVector3D nBottom = ccwNormal(verts[0], verts[1], verts[2]);    // SW, NW, NE
    QVector3D nNorth = ccwNormal(verts[7], verts[2], verts[1]);     // UNE, LNE, LNW
    QVector3D nSouth = ccwNormal(verts[0], verts[3], verts[6]);     // LSW, LSE, USE
    QVector3D nEast = ccwNormal(verts[7], verts[6], verts[3]);      // UNE, USE, LSE
    QVector3D nWest = ccwNormal(verts[0], verts[5], verts[4]);      // LSW, USW, UNW
    
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
