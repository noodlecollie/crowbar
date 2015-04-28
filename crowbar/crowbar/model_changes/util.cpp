#include "util.h"
#include <QVector3D>
#include <QMatrix4x4>

MODEL_BEGIN_NAMESPACE

namespace Util
{
    // Matrix to convert vectors from Hammer space to OpenGL space - the Y and Z co-ordinates are flipped
    // and the new Z axis is negated.
    static const QMatrix4x4 hammerToOpenGL4 = QMatrix4x4( 1.0,  0.0, 0.0, 0.0,
                                                          0.0,  0.0, 1.0, 0.0,
                                                          0.0, -1.0, 0.0, 0.0,
                                                          0.0,  0.0, 0.0, 1.0 );
    
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
    
    const QMatrix4x4 HammerToOpenGL()
    {
        return hammerToOpenGL4;
    }
    
    const QMatrix4x4 OpenGLToHammer()
    {
        return hammerToOpenGL4.inverted();
    }
}

MODEL_END_NAMESPACE
