#ifndef MODEL_UTIL_H
#define MODEL_UTIL_H

#include "model_global.h"
#include <QVector3D>
#include <QMatrix4x4>
#include "eulerangle.h"
#include <QPointer>

#define CLASSNAME(_class) _class::staticMetaObject.className()

MODEL_BEGIN_NAMESPACE

namespace Model_Util
{
    /**
      * @brief Compares two vectors using fuzzy floating-point comparison.
      * @param vec1 First vector to compare.
      * @param vec2 Second vector to compare.
      * @return True if vectors are considered equal, false otherwise.
      */
    bool MODELSHARED_EXPORT fuzzyVectorCompare(const QVector3D &vec1, const QVector3D &vec2);

    /**
      * @brief Converts a vector from Hammer co-ordinate space to OpenGL co-ordinate space
      * by swapping the Y and Z axis values.
      * @param vec Vector in Hammer space.
      * @return Vector converted to OpenGL space.
      */
    QVector3D MODELSHARED_EXPORT HammerToOpenGL(const QVector3D &vec);

    /**
      * @brief Converts a vector from OpenGL co-ordinate space to Hammer co-ordinate space.
      * @param vec Vector in OpenGL space.
      * @return Vector converted to Hammer space.
      */
    QVector3D MODELSHARED_EXPORT OpenGLToHammer(const QVector3D &vec);

    MODELSHARED_EXPORT const QMatrix4x4& OpenGLToHammer();
    MODELSHARED_EXPORT const QMatrix4x4& HammerToOpenGL();

    static const QVector3D VEC3_ORIGIN(0,0,0);

    // Default max and min vectors specifying the bounds for the map.
    // We need to read this in from an FGD at some point, but until then
    // these will suffice.
    QVector3D MODELSHARED_EXPORT mapCoordMin();
    QVector3D MODELSHARED_EXPORT mapCoordMax();

    float MODELSHARED_EXPORT clampToMultiple(float original, float multiple);

    float MODELSHARED_EXPORT remap(float input, float amin, float amax, float bmin, float bmax);

    void MODELSHARED_EXPORT multipleBounds(float value, float multiple, float &min, float &max);

    // Returns the closest power of two that is >= value.
    // Eg. if value = 60, return = 64.
    int MODELSHARED_EXPORT upperPowerOfTwo(qreal value);

    qreal MODELSHARED_EXPORT log2(qreal value);

    double MODELSHARED_EXPORT getRandomDouble(double min, double max);
    float MODELSHARED_EXPORT getRandomFloat(float min, float max);

    // Returns a matrix representing a translation by this vector.
    QMatrix4x4 MODELSHARED_EXPORT translationMatrix(const QVector3D &vec);

    // Returns a matrix representing the rotation of the vector ref
    // in order to point in the direction specified by ang.
    QMatrix4x4 MODELSHARED_EXPORT rotationMatrix(const EulerAngle &ang, const QVector3D &ref = QVector3D(1,0,0));

    // Returns a matrix representing a scale by this vector along X, Y and Z.
    QMatrix4x4 MODELSHARED_EXPORT scaleMatrix(const QVector3D &vec);

    qreal MODELSHARED_EXPORT angleBetween(const QVector3D &a, const QVector3D &b);

    // Removes any null QPointers from the given list.
    template<typename T>
    void clean(QVector<QPointer<T> > list);
}

MODEL_END_NAMESPACE

#endif // MODEL_UTIL_H
