#ifndef UTIL_H
#define UTIL_H

#include "model_global.h"

class QVector3D;
class QMatrix4x4;

MODEL_BEGIN_NAMESPACE

namespace Util
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
    
    const QMatrix4x4 MODELSHARED_EXPORT OpenGLToHammer();
    const QMatrix4x4 MODELSHARED_EXPORT HammerToOpenGL();
}

MODEL_END_NAMESPACE

#endif // UTIL_H
