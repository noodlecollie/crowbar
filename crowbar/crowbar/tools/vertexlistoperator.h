#ifndef VERTEXLISTOPERATOR_H
#define VERTEXLISTOPERATOR_H

#include <QObject>
#include "tools_global.h"
#include <QVector3D>
#include <QList>
#include <QQuaternion>
#include <QMatrix4x4>

TOOLS_BEGIN_NAMESPACE

class TOOLSSHARED_EXPORT VertexListOperator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QVector3D> vectorList READ vectorList WRITE setVectorList NOTIFY vectorListChanged)
    Q_PROPERTY(QMatrix4x4 operatorMatrix READ operatorMatrix WRITE setOperatorMatrix NOTIFY operatorMatrixChanged)
    Q_PROPERTY(QVector3D origin READ origin WRITE setOrigin NOTIFY originChanged)
    Q_PROPERTY(QQuaternion operatorQuaternion READ operatorQuaternion WRITE setOperatorQuaternion NOTIFY operatorQuaternionChanged)
public:
    explicit VertexListOperator(QObject *parent = 0);
    explicit VertexListOperator(const QList<QVector3D> &list, const QMatrix4x4 &mat,
                                const QVector3D &origin = QVector3D(), QObject* parent = 0);
    explicit VertexListOperator(const QList<QVector3D> &list, const QQuaternion &q,
                                const QVector3D &origin = QVector3D(), QObject* parent = 0);

    QList<QVector3D> vectorList() const;
    void setVectorList(const QList<QVector3D> &list);

    // If the operator matrix is specified, it will take priority.
    QMatrix4x4 operatorMatrix() const;
    void setOperatorMatrix(const QMatrix4x4 &mat);

    QQuaternion operatorQuaternion() const;
    void setOperatorQuaternion(const QQuaternion &q);

    QVector3D origin() const;
    void setOrigin(const QVector3D &origin);

    QList<QVector3D> performOperation() const;

signals:
    void vectorListChanged(const QList<QVector3D>&);
    void operatorMatrixChanged(const QMatrix4x4&);
    void originChanged(const QVector3D&);
    void operatorQuaternionChanged(const QQuaternion&);

public slots:

private:
    QList<QVector3D>    m_VectorList;           // Original, remains unmodified.
    QMatrix4x4          m_matOperatorMatrix;
    QVector3D           m_vecOrigin;
    QQuaternion         m_qOperatorQuaternion;
};

TOOLS_END_NAMESPACE

#endif // VERTEXLISTOPERATOR_H
