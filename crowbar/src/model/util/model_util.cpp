#include "model_util.h"
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>
#include <QtGlobal>

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

    // See: http://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d
    QMatrix4x4 rotationMatrix(const EulerAngle &ang, const QVector3D &ref)
    {
        QVector3D angVec = ang.toVector();
        if ( angVec == ref ) return QMatrix4x4();

        // Cross-product:
        QVector3D cross = QVector3D::crossProduct(ref, angVec);

        float sineTheta = cross.length();
        float cosineTheta = QVector3D::dotProduct(ref, angVec);
        float frac = (1.0 - cosineTheta)/(sineTheta * sineTheta);
        QMatrix3x3 cpmx;

        cpmx(0,0) = 0; cpmx(0,1) = -cross.z(); cpmx(0,2) = cross.y();
        cpmx(1,0) = cross.z(); cpmx(1,1) = 0; cpmx(1,2) = -cross.x();
        cpmx(2,0) = -cross.y(); cpmx(2,1) = cross.x(); cpmx(2,2) = 0;

        QMatrix3x3 result;
        result = result + cpmx + ((cpmx * cpmx) * frac);

        return QMatrix4x4(result(0,0), result(0,1), result(0,2), 0,
                          result(1,0), result(1,1), result(1,2), 0,
                          result(2,0), result(2,1), result(2,2), 0,
                          0, 0, 0, 1);
    }

    qreal angleBetween(const QVector3D &a, const QVector3D &b)
    {
        return qAcos((QVector3D::dotProduct(a,b))/(a.length() * b.length()));
    }
}

MODEL_END_NAMESPACE

