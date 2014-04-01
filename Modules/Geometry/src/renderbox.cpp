#include <Qt3D/QVector3DArray>

#include "renderbox.h"
#include "geomutil.h"
#include <QtDebug>

GEOMETRY_BEGIN_NAMESPACE

RenderBox::RenderBox() :
    QBox3D(), m_vecPosition(VEC3_ORIGIN), m_colColourMin(255,255,255,255), m_colColourMax(255,255,255,255)
{
}

RenderBox::RenderBox(const QVector3D &origin, const QVector3D &min, const QVector3D &max) :
    QBox3D(min, max), m_vecPosition(origin), m_colColourMin(255,255,255,255), m_colColourMax(255,255,255,255)
{
}

QColor RenderBox::colorMin() const
{
    return m_colColourMin;
}

void RenderBox::setColorMin(const QColor &col)
{
    m_colColourMin = col;
}

QColor RenderBox::colorMax() const
{
    return m_colColourMax;
}

void RenderBox::setColorMax(const QColor &col)
{
    m_colColourMax = col;
}

void RenderBox::setColor(const QColor &col)
{
    setColorMin(col);
    setColorMax(col);
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
    QColor colMin = colorMin();
    QColor colMax = colorMax();
    float colMinR = colMin.redF();
    float colMinG = colMin.greenF();
    float colMinB = colMin.blueF();
    float colMaxR = colMax.redF();
    float colMaxG = colMax.greenF();
    float colMaxB = colMax.blueF();
    QGeometryData geom;
    
    
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
        min,                                    // Lower SE 0
        QVector3D(max.x(), min.y(), min.z()),   // Lower SW 1
        QVector3D(max.x(), min.y(), max.z()),   // Lower NW 2
        QVector3D(min.x(), min.y(), max.z()),   // Lower NE 3
        QVector3D(max.x(), max.y(), min.z()),   // Upper SW 4
        QVector3D(min.x(), max.y(), min.z()),   // Upper SE 5
        QVector3D(min.x(), max.y(), max.z()),   // Upper NE 6
        max                                     // Upper NW 7
    };
    
    QColor cols[8] = 
    {
        QColor(0.0, 0.0, 0.0),
        QColor(0.0, 0.0, 0.0),
        QColor(0.0, 0.0, 0.0),
        QColor(0.0, 0.0, 0.0),
        QColor(0.0, 0.0, 0.0),
        QColor(0.0, 0.0, 0.0),
        QColor(0.0, 0.0, 0.0),
        QColor(0.0, 0.0, 0.0)
    };
    
    QVector3D dir = (max - min).normalized();
    
    // Set each vertex's colour appropriately.
    for ( int i = 0; i < 8; i++ )
    {
        float extent = scalarProjection((verts[i]-min).normalized(), dir);
        //qDebug() << "Extent for" << verts[i] << "=" << extent;
        cols[i].setRedF(colMinR + (extent * (colMaxR-colMinR)));
        cols[i].setGreenF(colMinG + (extent * (colMaxG-colMinG)));
        cols[i].setBlueF(colMinB + (extent * (colMaxB-colMinB)));
    }
    
    // Normals for faces
    const QVector3D nTop(0, 1, 0);      // Up    = +Y
    const QVector3D nBottom(0, -1, 0);  // Down  = -Y
    const QVector3D nNorth(0, 0, 1);    // North = +Z
    const QVector3D nSouth(0, 0, -1);   // South = -Z
    const QVector3D nEast(-1, 0, 0);    // East  = -X
    const QVector3D nWest(1, 0, 0);     // West  = +X

    int n = 0;
    // Top face
    geom.appendVertex(verts[4], verts[5], verts[6], verts[7]);
    geom.appendNormal(nTop, nTop, nTop, nTop);
    geom.appendColor(cols[4], cols[5], cols[6], cols[7]);
    geom.appendIndex(n);geom.appendIndex(n+1);geom.appendIndex(n+2);
    geom.appendIndex(n+2);geom.appendIndex(n+3);geom.appendIndex(n);
    
    n += 4;
    // Bottom face
    geom.appendVertex(verts[0], verts[1], verts[2], verts[3]);
    geom.appendNormal(nBottom, nBottom, nBottom, nBottom);
    geom.appendColor(cols[0], cols[1], cols[2], cols[3]);
    geom.appendIndex(n);geom.appendIndex(n+1);geom.appendIndex(n+2);
    geom.appendIndex(n+2);geom.appendIndex(n+3);geom.appendIndex(n);
    
    n += 4;
    // North face
    geom.appendVertex(verts[7], verts[2], verts[1], verts[4]);
    geom.appendNormal(nNorth, nNorth, nNorth, nNorth);
    geom.appendColor(cols[7], cols[2], cols[1], cols[4]);
    geom.appendIndex(n);geom.appendIndex(n+1);geom.appendIndex(n+2);
    geom.appendIndex(n+2);geom.appendIndex(n+3);geom.appendIndex(n);
    
    n += 4;
    // South face
    geom.appendVertex(verts[0], verts[3], verts[6], verts[5]);
    geom.appendNormal(nSouth, nSouth, nSouth, nSouth);
    geom.appendColor(cols[0], cols[3], cols[6], cols[5]);
    geom.appendIndex(n);geom.appendIndex(n+1);geom.appendIndex(n+2);
    geom.appendIndex(n+2);geom.appendIndex(n+3);geom.appendIndex(n);
    
    n += 4;
    // East face
    geom.appendVertex(verts[7], verts[6], verts[3], verts[2]);
    geom.appendNormal(nEast, nEast, nEast, nEast);
    geom.appendColor(cols[7], cols[6], cols[3], cols[2]);
    geom.appendIndex(n);geom.appendIndex(n+1);geom.appendIndex(n+2);
    geom.appendIndex(n+2);geom.appendIndex(n+3);geom.appendIndex(n);
    
    n += 4;
    // West face
    geom.appendVertex(verts[0], verts[5], verts[4], verts[1]);
    geom.appendNormal(nWest, nWest, nWest, nWest);
    geom.appendColor(cols[0], cols[5], cols[4], cols[1]);
    geom.appendIndex(n);geom.appendIndex(n+1);geom.appendIndex(n+2);
    geom.appendIndex(n+2);geom.appendIndex(n+3);geom.appendIndex(n);
    
    // Return the GeomData.
    return geom;
}

QBox3D RenderBox::boundingBox() const
{
    return QBox3D(minimum(), maximum());
}

bool RenderBox::_implementsIConstBBoxVolume() const
{
    return true;
}

GEOMETRY_END_NAMESPACE
