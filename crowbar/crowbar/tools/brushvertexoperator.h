#ifndef BRUSHVERTEXOPERATOR_H
#define BRUSHVERTEXOPERATOR_H

#include <QObject>
#include "tools_global.h"
#include "model_global.h"
#include <QMatrix4x4>
#include <QVector3D>
#include <QPointer>
#include <QQuaternion>
#include <QHash>

MODEL_BEGIN_NAMESPACE
class MapBrush;
MODEL_END_NAMESPACE

TOOLS_BEGIN_NAMESPACE

class TOOLSSHARED_EXPORT BrushVertexOperator : public QObject
{
    Q_OBJECT
public:
    struct OperationResults
    {
        QVector3D globalOriginPoition;
        QList<QVector3D> vertices;
    };

    explicit BrushVertexOperator(QObject *parent = 0);

    void addBrush(MODEL_NAMESPACE::MapBrush* brush);
    void clearBrushes();
    bool containsBrush(MODEL_NAMESPACE::MapBrush* brush) const;

    // If the operator matrix is specified, it will take priority.
    QMatrix4x4 operatorMatrix() const;
    void setOperatorMatrix(const QMatrix4x4 &mat);

    QVector3D origin() const;
    void setOrigin(const QVector3D &origin);

    // Key is original brush.
    // Value is transformed list of vertices for this brush.
    QHash<MODEL_NAMESPACE::MapBrush*, OperationResults> performOperation() const;
    
signals:
    void operatorMatrixChanged(const QMatrix4x4&);
    void originChanged(const QVector3D&);
    void brushListChanged();
    
public slots:
    
private:
    struct BrushInfo
    {
        QList<QVector3D> vertices;
        QPointer<MODEL_NAMESPACE::MapBrush> referencedBrush;
        QMatrix4x4 globalToLocal;
        QVector3D globalOriginPosition;
    };
    
    QList<BrushInfo>    m_BrushList;
    QMatrix4x4          m_matOperatorMatrix;
    QVector3D           m_vecOrigin;            // Is global.
};

TOOLS_END_NAMESPACE

#endif // BRUSHVERTEXOPERATOR_H
