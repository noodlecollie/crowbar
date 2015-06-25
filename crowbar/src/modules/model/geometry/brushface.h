#ifndef BRUSHFACE_H
#define BRUSHFACE_H

#include "model_global.h"
#include <QObject>
#include <QVector>
#include <QPointer>
#include <QVector3D>
#include <QVariant>

MODEL_BEGIN_NAMESPACE

class BrushVertex;

class MODELSHARED_EXPORT BrushFace : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant vertexIndices READ vertexIndices WRITE setVertexIndices NOTIFY vertexIndicesChanged)
    Q_PROPERTY(QVector3D normal READ normal)
public:
    explicit BrushFace(QObject *parent = 0);

    // QML only
    QVariant vertexIndices() const;
    void setVertexIndices(const QVariant &list);

    void verticesAppend(BrushVertex* vertex);
    BrushVertex* verticesItemAt(int index) const;
    int verticesCount() const;
    void verticesClear();
    void verticesClean();

    bool verticesContains(BrushVertex* vertex) const;
    void verticesInsertAt(int index, BrushVertex* vertex);
    void verticesRemoveAt(int index);
    
    QVector3D normal() const;
    bool isValid() const;

signals:
    void vertexIndicesChanged();

public slots:

private:
    QVector<QPointer<BrushVertex> > m_Vertices;
    QVector<int> m_VertexIndices;   // For QML only
};

MODEL_END_NAMESPACE

#endif // BRUSHFACE_H
