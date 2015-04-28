#include "brushvertexoperator.h"
#include "mapbrush.h"

using namespace MODEL_NAMESPACE;

TOOLS_BEGIN_NAMESPACE

BrushVertexOperator::BrushVertexOperator(QObject *parent) :
    QObject(parent), m_BrushList(), m_matOperatorMatrix(), m_vecOrigin()
{
}

void BrushVertexOperator::addBrush(MapBrush *brush)
{
    if ( !brush ) return;
    
    m_BrushList.append(BrushInfo());
    BrushInfo &info = m_BrushList.back();
    info.referencedBrush = brush;
    
    // We only need the vertices right now.
    for ( int i = 0; i < brush->vertices().count(); i++ )
    {
        info.vertices.append(brush->vertices(i)->position());
    }
    
    // Instead of using hierarchy just keep a cached versions of the matrices.
    info.globalToLocal = brush->hierarchyTransform()->globalTransformMatrix().inverted();
    info.globalOriginPosition = brush->hierarchyTransform()->globalTransformMatrix() * QVector3D(0,0,0);

    emit brushListChanged();
}

void BrushVertexOperator::clearBrushes()
{
    m_BrushList.clear();
    emit brushListChanged();
}

bool BrushVertexOperator::containsBrush(MapBrush *brush) const
{
    if ( !brush ) return false;
    
    for ( int i = 0; i < m_BrushList.count(); i++ )
    {
        if ( m_BrushList.at(i).referencedBrush.data() == brush ) return true;
    }
    
    return false;
}

QMatrix4x4 BrushVertexOperator::operatorMatrix() const
{
    return m_matOperatorMatrix;
}

void BrushVertexOperator::setOperatorMatrix(const QMatrix4x4 &mat)
{
    if ( mat == m_matOperatorMatrix ) return;

    m_matOperatorMatrix = mat;
    emit operatorMatrixChanged(m_matOperatorMatrix);
}

QVector3D BrushVertexOperator::origin() const
{
    return m_vecOrigin;
}

void BrushVertexOperator::setOrigin(const QVector3D &origin)
{
    if ( origin == m_vecOrigin ) return;

    m_vecOrigin = origin;
    emit originChanged(m_vecOrigin);
}

QHash<MapBrush*, BrushVertexOperator::OperationResults> BrushVertexOperator::performOperation() const
{
    QHash<MapBrush*, OperationResults> hash;
    
    for ( int i = 0; i < m_BrushList.count(); i++ )
    {
        const BrushInfo& info = m_BrushList.at(i);
        if ( info.referencedBrush.isNull() ) continue;
        
        OperationResults res;

        // Calculate the origin position.
        res.globalOriginPoition = (m_matOperatorMatrix * (info.globalOriginPosition - m_vecOrigin)) + m_vecOrigin;
        
        for ( int j = 0; j < info.vertices.count(); j++ )
        {
//            // Transform the vertex into global space.
//            QVector3D globalVertex = localToGlobal * info.vertices.at(j);
            
//            // Perform the transformation.
//            globalVertex = (m_matOperatorMatrix * (globalVertex - m_vecOrigin)) + m_vecOrigin;
            
//            // Transform back.
//            processedVertices.append(info.globalToLocal * globalVertex);
            res.vertices.append(m_matOperatorMatrix * info.vertices.at(j));
        }
        
        hash.insert(info.referencedBrush.data(), res);
    }
    
    return hash;
}

TOOLS_END_NAMESPACE
