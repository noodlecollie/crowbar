#include "brushmesh.h"
#include <QByteArray>
#include <Qt3DRenderer/QMeshData>
#include "brush.h"
#include "brushface.h"
#include "brushvertex.h"
#include <Qt3DRenderer/qbuffer.h>
#include <Qt3DRenderer/qattribute.h>

MODEL_BEGIN_NAMESPACE

BrushMesh::BrushMesh(QNode *parent) : Qt3D::QAbstractMesh(parent)
{
    m_pBrush = NULL;
    update();
}

Brush* BrushMesh::brush() const
{
    return m_pBrush;
}

void BrushMesh::setBrush(Brush *b)
{
    if ( m_pBrush == b ) return;
    
    m_pBrush = b;
    emit brushChanged(m_pBrush);
}

void BrushMesh::copy(const QNode *ref)
{
    QAbstractMesh::copy(ref);
    const BrushMesh *mesh = static_cast<const BrushMesh*>(ref);
    m_pBrush = mesh->m_pBrush;
}

Qt3D::QMeshDataPtr createBrushMesh(Brush* brush);

class BrushMeshFunctor : public Qt3D::QAbstractMeshFunctor
{
public:
    BrushMeshFunctor(const BrushMesh &c) : QAbstractMeshFunctor()
    {
        m_pBrush = c.brush();
    }
    
    Qt3D::QMeshDataPtr operator ()() Q_DECL_OVERRIDE
    {
        return m_pBrush ? createBrushMesh(m_pBrush) : Qt3D::QMeshDataPtr(new Qt3D::QMeshData());
    }

    bool operator ==(const QAbstractMeshFunctor &other) const Q_DECL_OVERRIDE
    {
        const BrushMeshFunctor* otherFunctor = dynamic_cast<const BrushMeshFunctor*>(&other);
        if (otherFunctor) return (otherFunctor->m_pBrush == m_pBrush);
        return false;
    }
    
private:
    Brush* m_pBrush;
};

Qt3D::QAbstractMeshFunctorPtr BrushMesh::meshFunctor() const
{
    return Qt3D::QAbstractMeshFunctorPtr(new BrushMeshFunctor(*this));
}

Qt3D::QMeshDataPtr createBrushMesh(Brush* brush)
{
    Q_ASSERT(brush);
    
    // Calculate size for each vertex entry in the buffer.
    // Right now we have position (3 floats) and normal (3 floats).
    const quint32 vertexDataFloats = 3 + 3;
    const quint32 stride = vertexDataFloats * sizeof(float);
    
    // Create buffers.
    QByteArray vertexBytes;
    QByteArray indexBytes;
    
    // Unfortunately we can't utilise indices because faces define the normals, so even when
    // a vertex is shared its normal will be different depending on the face.
    // Perhaps there's a way around this with shaders?
    int numVertices = brush->totalFaceVertices();
    vertexBytes.resize(stride * numVertices);
    indexBytes.resize(sizeof(quint16) * numVertices);
    
    // Apparently you can do this.
    float* vertices = reinterpret_cast<float*>(vertexBytes.data());
    quint16* indices = reinterpret_cast<quint16*>(indexBytes.data());
    
    // Populate the arrays.
    quint16 currentIndex = 0;
    for ( int i = 0; i < brush->facesCount(); i++ )
    {
        BrushFace* f = brush->facesItemAt(i);
        QVector3D normal = f->normal();
        float n[3];
        n[0] = normal.x();
        n[1] = normal.y();
        n[2] = normal.z();
        
        for ( int j = 0; j < f->verticesCount(); j++ )
        {
            QVector3D pos = f->verticesItemAt(j)->position();
            
            // Copy position data.
            *vertices = pos.x();
            *(vertices + sizeof(float)) = pos.y();
            *(vertices + (2*sizeof(float))) = pos.z();
            
            // Copy in the normal.
            memcpy(vertices + (3*sizeof(float)), n, 3 * sizeof(float));
            
            vertices += stride;
            
            *indices = currentIndex++;
            indices += sizeof(quint16);
        }
    }
    
    // Wrap the buffers.
    Qt3D::BufferPtr vertexBuffer(new Qt3D::Buffer(QOpenGLBuffer::VertexBuffer));
    vertexBuffer->setUsage(QOpenGLBuffer::DynamicDraw);
    vertexBuffer->setData(vertexBytes);
    
    Qt3D::BufferPtr indexBuffer(new Qt3D::Buffer(QOpenGLBuffer::IndexBuffer));
    indexBuffer->setUsage(QOpenGLBuffer::DynamicDraw);
    indexBuffer->setData(indexBytes);
    
    // Create the actual mesh data.
    Qt3D::QMeshDataPtr mesh(new Qt3D::QMeshData(Qt3D::QMeshData::Triangles));
    quint32 offset = 0;
    
    mesh->addAttribute(Qt3D::QMeshData::defaultPositionAttributeName(),
                       Qt3D::AttributePtr(new Qt3D::Attribute(vertexBuffer, GL_FLOAT_VEC3, numVertices, offset, stride)));
    offset += 3 * sizeof(float);
    
    mesh->addAttribute(Qt3D::QMeshData::defaultNormalAttributeName(),
                       Qt3D::AttributePtr(new Qt3D::Attribute(vertexBuffer, GL_FLOAT_VEC3, numVertices, offset, stride)));
    offset += 3 * sizeof(float);
    
    mesh->setIndexAttribute(Qt3D::AttributePtr(new Qt3D::Attribute(indexBuffer, GL_UNSIGNED_SHORT, numVertices, 0, 0)));
    
    mesh->computeBoundsFromAttribute(Qt3D::QMeshData::defaultPositionAttributeName());
    
    return mesh;
}

MODEL_END_NAMESPACE
