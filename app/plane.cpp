#include "plane.h"

void Plane::getPoints(QVector3D points[]) const
{
    for ( int i = 0; i < 3; i++ )
    {
        points[i] = m_Points[i];
    }
}

void Plane::getPoints(QList<QVector3D> &points) const
{
    for ( int i = 0; i < 3; i++ )
    {
        points.append(m_Points[i]);
    }
}

void Plane::setPoints(const QVector3D points[])
{
    for ( int i = 0; i < 3; i++ )
    {
        m_Points[i] = points[i];
    }

    m_bIsValid = checkValid(m_Points);

    if ( m_bIsValid )
    {
        m_Normal = QVector3D::normal(m_Points[1] - m_Points[0], m_Points[2] - m_Points[0]);   // 2-1, 3-1 is the same way Qt calculates planes.
    }
    else m_Normal = VEC3_ORIGIN;
}

void Plane::setPoints(const QList<QVector3D> &points)
{
    Q_ASSERT(points.size() >= 3);

    for ( int i = 0; i < 3; i++ )
    {
        m_Points[i] = points.at(i);
    }
}

void Plane::setPoints(const QVector3D p1, const QVector3D p2, const QVector3D p3)
{
    QVector3D points[] = {p1, p2, p3};
    setPoints(points);
}

bool Plane::checkValid(const QVector3D points[])
{
    return Plane::checkValid(points[0], points[1], points[2]);
}

bool Plane::checkValid(QList<QVector3D> &points)
{
    Q_ASSERT(points.size() >= 3);
    return checkValid(points.at(0), points.at(1), points.at(2));
}

bool Plane::checkValid(QVector3D p1, QVector3D p2, QVector3D p3)
{
    if ( p1 != p2 &&
         p1 != p3 &&
         p2 != p3 ) return true;

    return false;
}
