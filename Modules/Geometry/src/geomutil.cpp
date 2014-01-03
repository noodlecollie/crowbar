#include <QVector3D>
#include <QVector2D>
#include "geomutil.h"

GEOMETRY_BEGIN_NAMESPACE

const QVector3D VEC3_ORIGIN(0.0, 0.0, 0.0);
const QVector2D VEC2_ORIGIN(0.0, 0.0);

QVector3D ccwNormal(const QVector3D v0, const QVector3D v1, const QVector3D v2)
{
    return QVector3D::normal(v2-v1, v0-v1);
}

GEOMETRY_END_NAMESPACE
