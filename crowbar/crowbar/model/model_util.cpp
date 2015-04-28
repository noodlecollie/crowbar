#include "model_util.h"
#include <QtMath>
#include <QtGlobal>
#include <QtDebug>

MODEL_BEGIN_NAMESPACE

namespace Model_Util
{
    // Matrix to convert vectors from Hammer space to OpenGL space - the Y and Z co-ordinates are flipped
    // and the new Z axis is negated.
    static const QMatrix4x4 hammerToOpenGL4 = QMatrix4x4( 1.0,  0.0, 0.0, 0.0,
                                                          0.0,  0.0, 1.0, 0.0,
                                                          0.0, -1.0, 0.0, 0.0,
                                                          0.0,  0.0, 0.0, 1.0 );

    static const QMatrix4x4 hammerToOpenGL4i = hammerToOpenGL4.inverted();

    bool fuzzyVectorCompare(const QVector3D &vec1, const QVector3D &vec2)
    {
        return ( (qFuzzyIsNull(vec1.x()) && qFuzzyIsNull(vec2.x())) || qFuzzyCompare(vec1.x(), vec2.x())) &&
               ( (qFuzzyIsNull(vec1.y()) && qFuzzyIsNull(vec2.y())) || qFuzzyCompare(vec1.y(), vec2.y())) &&
               ( (qFuzzyIsNull(vec1.z()) && qFuzzyIsNull(vec2.z())) || qFuzzyCompare(vec1.z(), vec2.z()));
    }

    QVector3D HammerToOpenGL(const QVector3D &vec)
    {
        return QVector3D(vec.x(), vec.z(), vec.y());
    }

    QVector3D OpenGLToHammer(const QVector3D &vec)
    {
        return QVector3D(vec.x(), vec.z(), vec.y());
    }

    const QMatrix4x4& HammerToOpenGL()
    {
        return hammerToOpenGL4;
    }

    const QMatrix4x4& OpenGLToHammer()
    {
        return hammerToOpenGL4i;
    }

    QVector3D mapCoordMin()
    {
        return QVector3D(-16384, -16384, -16384);
    }

    QVector3D mapCoordMax()
    {
        return QVector3D(16384, 16384, 16384);
    }

    float clampToMultiple(float original, float multiple)
    {
        float division = original / multiple;
        float remainder = (division = qFloor(division)) * multiple;
        float lowerBound = original - remainder;
        float upperBound = lowerBound + multiple;
        float frac = (original - lowerBound)/(upperBound - lowerBound);
        if ( frac < 0.5f ) return lowerBound;
        else return upperBound;
    }

    float remap(float input, float amin, float amax, float bmin, float bmax)
    {
        // Technically this is undefined, but let's just return the mid of bmin and bmax.
        if ( amin == amax ) return (bmax - bmin)/2.0f;

        return bmin + ((input - amin)/(amax - amin) * (bmax - bmin));
    }

    void multipleBounds(float value, float multiple, float &min, float &max)
    {
        float division = value / multiple;
        float remainder = division - qFloor(division);

        if ( qFuzzyIsNull(remainder) )
        {
            max = value;
            min = value;
        }
        else
        {
            max = (value + (multiple - (remainder * multiple)));
            min = (value - (remainder * multiple));
        }
    }

    int upperPowerOfTwo(qreal value)
    {
        return (int)qPow(2, qCeil(log2(value)));
    }

    qreal log2(qreal value)
    {
        return qLn(value)/qLn(2);
    }

    double getRandomDouble(double min, double max)
    {
        return min + (((double)qrand()/(double)RAND_MAX) * (max-min));
    }

    float getRandomFloat(float min, float max)
    {
        return (float)getRandomDouble(min, max);
    }

    QMatrix4x4 scaleMatrix(const QVector3D &vec)
    {
        return QMatrix4x4(vec.x(), 0, 0, 0,
                          0, vec.y(), 0, 0,
                          0, 0, vec.z(), 0,
                          0, 0, 0, 1);
    }

    QMatrix4x4 translationMatrix(const QVector3D &vec)
    {
        return QMatrix4x4(1, 0, 0, vec.x(),
                          0, 1, 0, vec.y(),
                          0, 0, 1, vec.z(),
                          0, 0, 0, 1);
    }

    QMatrix4x4 rotationMatrix(const EulerAngle &ang)
    {
        return rotationMatrixZ(ang.yaw()) * rotationMatrixY(ang.pitch()) * rotationMatrixX(ang.roll());
    }
    
    QMatrix4x4 rotationMatrixX(float degrees)
    {
        float rad = qDegreesToRadians(degrees);
        float c = qCos(rad);
        float s = qSin(rad);
        
        return QMatrix4x4( 1, 0,  0, 0,
                           0, c, -s, 0,
                           0, s,  c, 0,
                           0, 0,  0, 1 );
    }
    
    QMatrix4x4 rotationMatrixY(float degrees)
    {
        float rad = qDegreesToRadians(degrees);
        float c = qCos(rad);
        float s = qSin(rad);
        
        return QMatrix4x4(  c, 0, s, 0,
                            0, 1, 0, 0,
                           -s, 0, c, 0,
                            0, 0, 0, 1 );
    }
    
    QMatrix4x4 rotationMatrixZ(float degrees)
    {
        float rad = qDegreesToRadians(degrees);
        float c = qCos(rad);
        float s = qSin(rad);
        
        return QMatrix4x4( c, -s, 0, 0,
                           s,  c, 0, 0,
                           0,  0, 1, 0,
                           0,  0, 0, 1 );
    }

    qreal angleBetween(const QVector3D &a, const QVector3D &b)
    {
        return qAcos((QVector3D::dotProduct(a,b))/(a.length() * b.length()));
    }

    float snapPowerTwo(int power, float value)
    {
        // The multiple we're using.
        float powerTwo = qPow(2.0f, (float)power);
        float min = 0.0f, max = 0.0f;

        // Get the two bounds either side of the value for this multiple.
        multipleBounds(value, powerTwo, min, max);

        // Return the bound that's nearest.
        return (value - min) < (max - value) ? min : max;
    }

    QVector3D snapPowerTwo(int power, const QVector3D &vec)
    {
        return QVector3D(snapPowerTwo(power, vec.x()),
                         snapPowerTwo(power, vec.y()),
                         snapPowerTwo(power, vec.z()));
    }
    QVector4D snapPowerTwo(int power, const QVector4D &vec)
    {
        return QVector4D(snapPowerTwo(power, vec.x()),
                         snapPowerTwo(power, vec.y()),
                         snapPowerTwo(power, vec.z()),
                         1);
    }
    
    QVector2D snapPowerTwo(int power, const QVector2D &vec)
    {
        return QVector2D(snapPowerTwo(power, vec.x()),
                         snapPowerTwo(power, vec.y()));
    }

    QMatrix4x4 clientToView(const QSize &viewSize)
    {
        return QMatrix4x4( 2.0f/(float)viewSize.width(), 0, 0, (1.0f - (float)viewSize.width())/(float)viewSize.width(),
                           0, -2.0f/(float)viewSize.height(), 0, ((float)viewSize.height() - 1.0f)/viewSize.height(),
                           0, 0, 1, 0,
                           0, 0, 0, 1 );
    }

    QMatrix4x4 viewToGrid(float zoom, const QSize &viewSize, const QVector2D viewOriginOnGrid)
    {
        return QMatrix4x4( (zoom*(float)viewSize.width())/2.0f, 0, 0, viewOriginOnGrid.x(),
                           0, (zoom*(float)viewSize.height())/2.0f, 0, viewOriginOnGrid.y(),
                           0, 0, 1, 0,
                           0, 0, 0, 1 );
    }

     QMatrix4x4 gridToWorld(const QVector3D &xBasis, const QVector3D &yBasis)
     {
        QVector3D cross = QVector3D::crossProduct(xBasis, yBasis);
        return QMatrix4x4( xBasis.x(), yBasis.x(), cross.x(), 0,
                           xBasis.y(), yBasis.y(), cross.y(), 0,
                           xBasis.z(), yBasis.z(), cross.z(), 0,
                           0, 0, 0, 1 );
     }
     
     QMatrix4x4 clientToGrid(float zoom, const QSize &viewSize, const QVector2D viewOriginOnGrid)
     {
         return QMatrix4x4( zoom, 0, 0, ((zoom*(1.0f - (float)viewSize.width()))/2.0f) + viewOriginOnGrid.x(),
                            0, -zoom, 0, ((zoom*((float)viewSize.height() - 1.0f))/2.0f) + viewOriginOnGrid.y(),
                            0, 0, 1, 0,
                            0, 0, 0, 1 );
     }
     
     QVector2D snapToGrid(const QPoint &pixel, int gridPower, float gridZoom, const QSize &viewSize, const QVector2D viewCentreOnGrid)
     {
         return snapToGrid(QVector2D(pixel.x(), pixel.y()), gridPower, gridZoom, viewSize, viewCentreOnGrid);
     }
     
     QVector2D snapToGrid(const QVector2D &vec, int gridPower, float gridZoom, const QSize &viewSize, const QVector2D viewCentreOnGrid)
     {
         QMatrix4x4 clientToGrid = Model_Util::clientToGrid(gridZoom, viewSize, viewCentreOnGrid);
         QMatrix4x4 gridToClient = clientToGrid.inverted();
         
         QVector4D v(vec.x(), vec.y(), 0, 1);
         
         // Convert to grid space.
         v = clientToGrid * v;
         
         // Snap to gridlines.
         v = Model_Util::snapPowerTwo(gridPower, v);
         
         // Convert back.
         v = gridToClient * v;
         
         return QVector2D(v.x(), v.y());
     }
     
     QVector3D snapToGrid(const QVector3D &vec, int gridPower, float gridZoom, const QSize &viewSize, const QVector2D viewCentreOnGrid)
     {
         QVector2D v = snapToGrid(QVector2D(vec.x(), vec.y()), gridPower, gridZoom, viewSize, viewCentreOnGrid);
         return QVector3D(v.x(), v.y(), vec.z());
     }
     
     QVector2D multiply(const QMatrix4x4 &mat, const QVector2D &vec)
     {
         QVector3D v(vec.x(), vec.y(), 0);
         v = mat * v;
         return QVector2D(v.x(), v.y());
     }
     
     void cheapTextureAxes(const QVector3D &vec, QVector3D &uAxis, QVector3D &vAxis)
     {
         float dotX = qAbs<float>(QVector3D::dotProduct(vec, QVector3D(1,0,0)));
         float dotY = qAbs<float>(QVector3D::dotProduct(vec, QVector3D(0,1,0)));
         float dotZ = qAbs<float>(QVector3D::dotProduct(vec, QVector3D(0,0,1)));
         
         if ( dotX >= dotY && dotX >= dotZ )
         {
             uAxis = QVector3D(0,1,0);
             vAxis = QVector3D(0,0,1);
         }
         else if ( dotY >= dotX && dotY >= dotZ )
         {
             uAxis = QVector3D(1,0,0);
             vAxis = QVector3D(0,0,1);
         }
         else
         {
             uAxis = QVector3D(1,0,0);
             vAxis = QVector3D(0,1,0);
         }
     }
     
     QMatrix4x4 texelToTextureCoordinate(int textureWidth, int textureHeight)
     {
         return QMatrix4x4( 1.0f/(float)textureWidth, 0, 0, 1.0f/(2.0f * (float)textureWidth),
                            0, -1.0f/(float)textureHeight, 0, (1.0f - (1.0f/(2.0f * (float)textureHeight))),
                            0, 0, 1, 0,
                            0, 0, 0, 1 );
     }
     
     QMatrix4x4 texelCornerToTextureCoordinate(int textureWidth, int textureHeight)
     {
         return QMatrix4x4( 1.0f/(float)textureWidth, 0, 0, 0,
                            0, -1.0f/(float)textureHeight, 0, 1,
                            0, 0, 1, 0,
                            0, 0, 0, 1 );
     }
     
     QVector2D textureCoordinatesPerTexel(int textureWidth, int textureHeight)
     {
         return QVector2D(1.0f/(float)textureWidth, 1.0f/(float)textureHeight);
     }
     
     bool hasZeroComponent(const QVector3D &vec)
     {
         return vec.x() == 0.0f || vec.y() == 0.0f || vec.z() == 0.0f;
     }
     
     bool hasZeroComponent(const QVector2D &vec)
     {
         return vec.x() == 0.0f || vec.y() == 0.0f;
     }
     
     bool hasZeroComponent(const QVector3D &vec1, const QVector3D &vec2)
     {
         return hasZeroComponent(vec2-vec1);
     }
     
     bool hasZeroComponent(const QVector2D &vec1, const QVector2D &vec2)
     {
         return hasZeroComponent(vec2-vec1);
     }
     
     bool fuzzyHasZeroComponent(const QVector3D &vec)
     {
         return qFuzzyIsNull(vec.x()) || qFuzzyIsNull(vec.y()) || qFuzzyIsNull(vec.z());
     }
     
     bool fuzzyHasZeroComponent(const QVector2D &vec)
     {
         return qFuzzyIsNull(vec.x()) || qFuzzyIsNull(vec.y());
     }
     
     bool fuzzyHasZeroComponent(const QVector3D &vec1, const QVector3D &vec2)
     {
         return fuzzyHasZeroComponent(vec2-vec1);
     }
     
     bool fuzzyHasZeroComponent(const QVector2D &vec1, const QVector2D &vec2)
     {
         return fuzzyHasZeroComponent(vec2-vec1);
     }
     
     void transformOrientation(const QMatrix4x4 &transform,
                               QVector3D &transOut, EulerAngle &rotOut, QVector3D &scaleOut)
     {
         // Begin with origin and default direction in local space.
         QVector3D origin(0,0,0);
         QVector3D directionX(1,0,0);
         QVector3D directionY(0,1,0);
         QVector3D directionZ(0,0,1);
         
         // Transform these into the new system.
         origin = transform * origin;
         directionX = transform * directionX;
         directionY = transform * directionY;
         directionZ = transform * directionZ;
         
         // The orientation process is scale -> rotate -> translate.
         // The translation just becomes the new position of the origin.
         transOut = origin;
         
         // The scale should be given by the lengths of the new vectors.
         scaleOut.setX((directionX - origin).length());
         scaleOut.setY((directionY - origin).length());
         scaleOut.setZ((directionZ - origin).length());
         
         // The rotation will be given by:
         rotOut = EulerAngle::vectorToAngle(directionX - origin);
     }
}

MODEL_END_NAMESPACE
