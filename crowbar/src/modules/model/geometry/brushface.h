#ifndef BRUSHFACE_H
#define BRUSHFACE_H

#include "model_global.h"
#include <QObject>
#include <QVector>
#include <QPointer>

MODEL_BEGIN_NAMESPACE

class BrushVertex;

class MODELSHARED_EXPORT BrushFace : public QObject
{
    Q_OBJECT
public:
    explicit BrushFace(QObject *parent = 0);

    void verticesAppend(BrushVertex* vertex);
    BrushVertex* verticesItemAt(int index) const;
    int verticesCount() const;
    void verticesClear();

signals:

public slots:

private:
    QVector<QPointer<BrushVertex> > m_Vertices;
};

MODEL_END_NAMESPACE

#endif // BRUSHFACE_H
