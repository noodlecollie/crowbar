#ifndef MODEL_UTIL_H
#define MODEL_UTIL_H

#include "model_global.h"
#include <QVector3D>
#include <QVector2D>
#include <QVector4D>
#include <QMatrix4x4>
#include "eulerangle.h"

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

    // Returns a matrix representing a rotation by these angles, relative to the (0,0,0) angle (ie (1,0,0) vector).
    QMatrix4x4 MODELSHARED_EXPORT rotationMatrix(const EulerAngle &ang);
    
    QMatrix4x4 MODELSHARED_EXPORT rotationMatrixX(float degrees);
    QMatrix4x4 MODELSHARED_EXPORT rotationMatrixY(float degrees);
    QMatrix4x4 MODELSHARED_EXPORT rotationMatrixZ(float degrees);

    // Returns a matrix representing a scale by this vector along X, Y and Z.
    QMatrix4x4 MODELSHARED_EXPORT scaleMatrix(const QVector3D &vec);

    qreal MODELSHARED_EXPORT angleBetween(const QVector3D &a, const QVector3D &b);

    float MODELSHARED_EXPORT snapPowerTwo(int power, float value);
    QVector3D MODELSHARED_EXPORT snapPowerTwo(int power, const QVector3D &vec);
    QVector4D MODELSHARED_EXPORT snapPowerTwo(int power, const QVector4D &vec);
    QVector2D MODELSHARED_EXPORT snapPowerTwo(int power, const QVector2D &vec);
    
    QVector2D MODELSHARED_EXPORT snapToGrid(const QPoint &pixel, int gridPower, float gridZoom,
                                            const QSize &viewSize, const QVector2D viewCentreOnGrid);
    
    QVector2D MODELSHARED_EXPORT snapToGrid(const QVector2D &vec, int gridPower, float gridZoom,
                                            const QSize &viewSize, const QVector2D viewCentreOnGrid);
    
    // Z component is not changed.
    QVector3D MODELSHARED_EXPORT snapToGrid(const QVector3D &vec, int gridPower, float gridZoom,
                                            const QSize &viewSize, const QVector2D viewCentreOnGrid);

    /**
      == Translations between co-ordinate spaces ==
      
      Client space: in pixels, origin top left, X goes right, Y goes down, size (w,h).
      View space: origin in centre, X goes right, Y goes up, min (-1,-1), max (1,1).
      Grid space: View origin on grid (vx,vy), grid units per pixel (z,z), grid
                  X and Y vectors (xa,xb,xc) and (ya,yb,yc) in world co-ordinates.
                  Depth vector given by cross-product of X and Y: (za,zb,zc). Depth
                  comes out of the screen - do not confuse this with the view direction,
                  which is the opposite!
      World space: World origin is at grid origin.

      Client to view:
      - Translate by (-(w-1)/2, -(h-1)/2) because client co-ords are at centre of pixels
        This is the same as (1-w/2, 1-h/2)
      - Scale by (2/w, -2/h)
      [ 2/w   0  0 1-w/w ]
      [  0  -2/h 0 h-1/h ]
      [  0    0  1   0   ]
      [  0    0  0   1   ]

      View to grid:
      - Scale by view units per pixel (zw/2, zh/2)
      - Translate by view origin translation (vx, vy)
      [ zw/2   0  0 vx ]
      [   0  zh/2 0 vy ]
      [   0    0  1  0 ]
      [   0    0  0  1 ]

      Grid to world:
      [ xa xb xc 0 ]
      [ ya yb yc 0 ]
      [ za zb zc 0 ]
      [  0  0  0 1 ]
      
      Client to grid:
      This method may retain more precision than using client -> view -> grid.
      Centre of client = (w-1/2, h-1/2)
      Grid units per pixel = (z, z)
      Position of this centre on grid = (vx, vy)
      - Translate by (-(w-1/2), -(h-1/2))
      - Scale by (z, -z) to flip the Y axis.
      - Translate by (vx, vy)
      [ z  0 0 (z(1-w)/2) + vx ]
      [ 0 -z 0 (z(h-1)/2) + vy ]
      [ 0  0 1        0        ]
      [ 0  0 0        1        ]

      */

    QMatrix4x4 MODELSHARED_EXPORT clientToView(const QSize &viewSize);
    QMatrix4x4 MODELSHARED_EXPORT viewToGrid(float zoom, const QSize &viewSize, const QVector2D viewOriginOnGrid);
    QMatrix4x4 MODELSHARED_EXPORT gridToWorld(const QVector3D &xBasis, const QVector3D &yBasis);
    QMatrix4x4 MODELSHARED_EXPORT clientToGrid(float zoom, const QSize &viewSize, const QVector2D viewOriginOnGrid);
    
    QVector2D MODELSHARED_EXPORT multiply(const QMatrix4x4 &mat, const QVector2D &vec);
    void MODELSHARED_EXPORT cheapTextureAxes(const QVector3D &vec, QVector3D &uAxis, QVector3D &vAxis);
    
    // Treats the texel co-ordinate as being in the centre of the texel.
    QMatrix4x4 MODELSHARED_EXPORT texelToTextureCoordinate(int textureWidth, int textureHeight);
    
    // Treats the texel co-ordinate as being at the top left corner of the texel.
    QMatrix4x4 MODELSHARED_EXPORT texelCornerToTextureCoordinate(int textureWidth, int textureHeight);
    
    // Ignores different texel vs. texture co-ordinate axis directions
    // (ie. values returned are scalar).
    QVector2D MODELSHARED_EXPORT textureCoordinatesPerTexel(int textureWidth, int textureHeight);
    
    bool MODELSHARED_EXPORT hasZeroComponent(const QVector3D &vec);
    bool MODELSHARED_EXPORT hasZeroComponent(const QVector2D &vec);
    bool MODELSHARED_EXPORT hasZeroComponent(const QVector3D &vec1, const QVector3D &vec2);
    bool MODELSHARED_EXPORT hasZeroComponent(const QVector2D &vec1, const QVector2D &vec2);
    
    bool MODELSHARED_EXPORT fuzzyHasZeroComponent(const QVector3D &vec);
    bool MODELSHARED_EXPORT fuzzyHasZeroComponent(const QVector2D &vec);
    bool MODELSHARED_EXPORT fuzzyHasZeroComponent(const QVector3D &vec1, const QVector3D &vec2);
    bool MODELSHARED_EXPORT fuzzyHasZeroComponent(const QVector2D &vec1, const QVector3D &vec2);
    
    // Given a matrix transform from the original co-ordinate system to the new one,
    // outputs the relevant orientation parameters.
    void MODELSHARED_EXPORT transformOrientation(const QMatrix4x4 &transform,
                                                 QVector3D &transOut, EulerAngle &rotOut, QVector3D &scaleOut);
}

MODEL_END_NAMESPACE

#endif // MODEL_UTIL_H
