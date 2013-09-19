/*! \file plane.h
 * \brief Defines a plane class to hold the co-ordinates of a plane in 3D space.
 *
 * The plane class contains three points that define the plane and a normal that defines its front.
 * Planes can be invalid, which means that two or more of their defining points are identical.
 */

#ifndef PLANE_H
#define PLANE_H

#include "matlib.h"
#include <QVector3D>
#include <QList>

/**
 * @brief Defines a plane in 3D space.
 */
class Plane
{
public:
    /**
     * @brief Constructor. Member variables are set to zero values.
     */
    Plane() : m_Points(), m_Normal(VEC3_ORIGIN), m_bIsValid(false)
    {
        m_Points[0] = VEC3_ORIGIN;
        m_Points[1] = VEC3_ORIGIN;
        m_Points[2] = VEC3_ORIGIN;
    }

    /**
     * @brief Constructor specifying an array of plane points. The normal is calculated with vectors [1]-[0], [2]-[0].
     * @param points 3 points on the plane.
     */
    Plane(const QVector3D points[3]) : m_Points(), m_Normal(VEC3_ORIGIN), m_bIsValid(false)
    {
        setPoints(points);
    }

    /**
     * @brief Constructor specifying plane points. The normal is calculated with vectors p2-p1, p3-p1.
     * @param p1 Point 1.
     * @param p2 Point 2.
     * @param p3 Point 3.
     */
    Plane(const QVector3D p1, const QVector3D p2, const QVector3D p3) :
        m_Points(), m_Normal(VEC3_ORIGIN), m_bIsValid(false)
    {
        setPoints(p1, p2, p3);
    }

    // Get
    /**
     * @brief Gets an array of the three points on the plane.
     * @param points Array to fill with points.
     */
    inline void getPoints(QVector3D points[3]) const;

    /**
     * @brief Fills a QList with the three points on the plane.
     * @param points List to fill. Points are appended to the list.
     */
    inline void getPoints(QList<QVector3D> &points) const;

    /**
     * @brief Specifies whether the plane is valid. Invalid planes contain two or more identical plane points.
     * @return True if valid, false otherwise.
     */
    inline bool isValid() const                         { return m_bIsValid; }

    /**
     * @brief Returns the plane's normal, specifying the forward-facing direction of the plane.
     * @return Normalised direction vector representing the plane's normal.
     */
    inline QVector3D getNormal() const                  { return m_Normal; }

    // Set
    /**
     * @brief Sets the plane's points via an array. The normal is calculated with vectors [1]-[0], [2]-[0].
     * @param points Array of points to set.
     */
    void setPoints(const QVector3D points[3]);

    /**
     * @brief Sets the plane's points via a QList. The normal is calculated with vectors [1]-[0], [2]-[0].
     * @param points List of points to set.
     */
    void setPoints(const QList<QVector3D> &points);

    /**
     * @brief Sets the plane's points. The normal is calculated with vectors p2-p1, p3-p1.
     * @param p1 First point.
     * @param p2 Second point.
     * @param p3 Third point.
     */
    void setPoints(const QVector3D p1, const QVector3D p2, const QVector3D p3);

private:
    /**
     * @brief Returns whether the given points represent a valid plane.
     * @param points Points to check.
     * @return True if plane is valid, false otherwise.
     */
    static bool checkValid(const QVector3D points[3]);

    /**
     * @brief Returns whether the given points represent a valid plane.
     * @param points List of points to check. The first three elements are used.
     * @return True if plane is valid, false otherwise.
     */
    static bool checkValid(QList<QVector3D> &points);

    /**
     * @brief Returns whether the given points represent a valid plane.
     * @param p1 Point 1.
     * @param p2 Point 2.
     * @param p3 Point 3.
     * @return True if plane is valid, false otherwise.
     */
    static bool checkValid(QVector3D p1, QVector3D p2, QVector3D p3);

    QVector3D   m_Points[3];    /**< Points on the plane. */
    QVector3D   m_Normal;       /**< Normal of the plane. */
    bool        m_bIsValid;     /**< Whether the plane is valid or not. */
};

#endif // PLANE_H
