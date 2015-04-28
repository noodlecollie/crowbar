#include "vertexlistoperator.h"

TOOLS_BEGIN_NAMESPACE

VertexListOperator::VertexListOperator(QObject *parent) :
    QObject(parent)
{
}

VertexListOperator::VertexListOperator(const QList<QVector3D> &list, const QMatrix4x4 &mat,
                                       const QVector3D &origin, QObject *parent) :
    QObject(parent), m_VectorList(list), m_matOperatorMatrix(mat), m_vecOrigin(origin), m_qOperatorQuaternion()
{
}

VertexListOperator::VertexListOperator(const QList<QVector3D> &list, const QQuaternion &q,
                                       const QVector3D &origin, QObject *parent) :
    QObject(parent), m_VectorList(list), m_matOperatorMatrix(), m_vecOrigin(origin), m_qOperatorQuaternion(q)
{
}

QList<QVector3D> VertexListOperator::vectorList() const
{
    return m_VectorList;
}

void VertexListOperator::setVectorList(const QList<QVector3D> &list)
{
    m_VectorList = list;
    emit vectorListChanged(m_VectorList);
}

QMatrix4x4 VertexListOperator::operatorMatrix() const
{
    return m_matOperatorMatrix;
}

void VertexListOperator::setOperatorMatrix(const QMatrix4x4 &mat)
{
    if ( mat == m_matOperatorMatrix ) return;

    m_matOperatorMatrix = mat;
    emit operatorMatrixChanged(m_matOperatorMatrix);
}

QQuaternion VertexListOperator::operatorQuaternion() const
{
    return m_qOperatorQuaternion;
}

void VertexListOperator::setOperatorQuaternion(const QQuaternion &q)
{
    if ( q == m_qOperatorQuaternion ) return;

    m_qOperatorQuaternion = q;
    emit operatorQuaternionChanged(m_qOperatorQuaternion);
}

QVector3D VertexListOperator::origin() const
{
    return m_vecOrigin;
}

void VertexListOperator::setOrigin(const QVector3D &origin)
{
    if ( origin == m_vecOrigin ) return;

    m_vecOrigin = origin;
    emit originChanged(m_vecOrigin);
}

QList<QVector3D> VertexListOperator::performOperation() const
{
    if ( !m_matOperatorMatrix.isIdentity() )
    {
        QList<QVector3D> out;
        for ( int i = 0; i < m_VectorList.count(); i++ )
        {
            out.append(( m_matOperatorMatrix * (m_VectorList.at(i) - m_vecOrigin) ) + m_vecOrigin);
        }

        return out;
    }

    if ( !m_qOperatorQuaternion.isIdentity() )
    {
        QList<QVector3D> out;
        for ( int i = 0; i < m_VectorList.count(); i++ )
        {
            out.append(m_qOperatorQuaternion.rotatedVector(m_VectorList.at(i) - m_vecOrigin) + m_vecOrigin);
        }

        return out;
    }

    return m_VectorList;
}

TOOLS_END_NAMESPACE
