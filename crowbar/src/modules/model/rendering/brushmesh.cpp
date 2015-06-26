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
    update();
}

void BrushMesh::copy(const QNode *ref)
{
    QAbstractMesh::copy(ref);

    // TODO if we have anything to copy later.
    //const BrushMesh *mesh = static_cast<const BrushMesh*>(ref);
}

Qt3D::QMeshDataPtr createBrushMesh(Brush* brush);

class BrushMeshFunctor : public Qt3D::QAbstractMeshFunctor
{
public:
    BrushMeshFunctor(const BrushMesh &c) : QAbstractMeshFunctor()
    {
        m_pBrush = qobject_cast<Brush*>(c.parent());
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

int numIndices(BrushFace* face)
{
    // Because we can't draw using a single triangle fan for each face,
    // we just draw with dumb triangles instead.
    // This means the indices go 0 1 2; 0 2 3; 0 3 4; ...
    // The formula for the number of indices, given n vertices, is:

    return (3 * face->verticesCount()) - 6;
}

Qt3D::QMeshDataPtr createBrushMesh(Brush* brush)
{
    Q_ASSERT(brush);
    
    // Calculate size for each vertex entry in the buffer.
    // Right now we have position (3 floats) and normal (3 floats).
    const quint32 vertexDataFloats = 3 + 3;
    const quint32 stride = vertexDataFloats * sizeof(float);

    int totalIndices = 0;
    for ( int i = 0; i < brush->facesCount(); i++ )
    {
        totalIndices += numIndices(brush->facesItemAt(i));
    }
    
    // Create buffers.
    QByteArray vertexBytes;
    QByteArray indexBytes;
    
    // Unfortunately we can't utilise indices because faces define the normals, so even when
    // a vertex is shared its normal will be different depending on the face.
    // Perhaps there's a way around this with shaders?
    int numVertices = brush->totalFaceVertices();
    vertexBytes.resize(stride * numVertices);
    indexBytes.resize(sizeof(quint16) * totalIndices);
    
    // Cast the raw byte pointers to the correct types for easier assignment.
    float* vertices = reinterpret_cast<float*>(vertexBytes.data());
    quint16* indices = reinterpret_cast<quint16*>(indexBytes.data());
    
    // Populate the arrays.
    int currentVertIndex = 0;
    int verticesWritten = 0;
    int cIndex = 0;
    for ( int i = 0; i < brush->facesCount(); i++ )
    {
        BrushFace* f = brush->facesItemAt(i);
        int baseVertex = verticesWritten;

        QVector3D normal = f->normal();
        float n[6];
        n[3] = normal.x();
        n[4] = normal.y();
        n[5] = normal.z();
        
        for ( int j = 0; j < f->verticesCount(); j++ )
        {
            QVector3D pos = f->verticesItemAt(j)->position();
            
            // Copy position data.
            n[0] = pos.x();
            n[1] = pos.y();
            n[2] = pos.z();
            
            // Copy in all the data.
            memcpy(&vertices[currentVertIndex], n, 6 * sizeof(float));
            currentVertIndex += vertexDataFloats;
            verticesWritten++;

            if ( j >= 2 )
            {
                indices[cIndex++] = baseVertex;
                indices[cIndex++] = baseVertex + j - 1;
                indices[cIndex++] = baseVertex + j;
            }
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
    
    mesh->addAttribute(Qt3D::QMeshData::defaultPositionAttributeName(),
                       Qt3D::AttributePtr(new Qt3D::Attribute(vertexBuffer, GL_FLOAT_VEC3, numVertices, 0, stride)));
    
    mesh->addAttribute(Qt3D::QMeshData::defaultNormalAttributeName(),
                       Qt3D::AttributePtr(new Qt3D::Attribute(vertexBuffer, GL_FLOAT_VEC3, numVertices, 3 * sizeof(float), stride)));
    
    mesh->setIndexAttribute(Qt3D::AttributePtr(new Qt3D::Attribute(indexBuffer, GL_UNSIGNED_SHORT, totalIndices, 0, 0)));
    
    mesh->computeBoundsFromAttribute(Qt3D::QMeshData::defaultPositionAttributeName());
    
    return mesh;
}

MODEL_END_NAMESPACE
