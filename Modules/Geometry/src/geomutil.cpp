#include <QVector3D>
#include <QVector2D>
#include "geomutil.h"
#include <Qt3D/QBox3D>

GEOMETRY_BEGIN_NAMESPACE

const QVector3D VEC3_ORIGIN(0.0, 0.0, 0.0);
const QVector2D VEC2_ORIGIN(0.0, 0.0);

QVector3D ccwNormal(const QVector3D &v0, const QVector3D &v1, const QVector3D &v2)
{
    return QVector3D::normal(v2-v1, v0-v1);
}

qreal scalarProjection(const QVector3D &v0, const QVector3D &v1)
{
    return (QVector3D::dotProduct(v0, v1))/(v1.length());
}

QVector3D vectorProjection(const QVector3D &v0, const QVector3D &v1)
{
    return scalarProjection(v0, v1) * v1.normalized();
}

bool boundingBoxesIntersect(const QVector3D &max1, const QVector3D &min1, const QVector3D &max2, const QVector3D &min2)
{
    if ( max1.x() < min2.x() || max2.x() < min1.x() ||
         max1.y() < min2.y() || max2.y() < min1.y() ||
         max1.z() < min2.z() || max2.z() < min1.z() )
    {
        return false;
    }
    
    return true;
}

bool boundingBoxesIntersect(const QBox3D &box1, const QBox3D &box2)
{
    return boundingBoxesIntersect(box1.maximum(), box1.minimum(), box2.maximum(), box2.minimum());
}

void removeme() {}

GEOMETRY_END_NAMESPACE
