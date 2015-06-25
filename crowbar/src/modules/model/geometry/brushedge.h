#ifndef BRUSHEDGE_H
#define BRUSHEDGE_H

#include "model_global.h"
#include <QObject>
#include <QPointer>

MODEL_BEGIN_NAMESPACE

class BrushVertex;

class MODELSHARED_EXPORT BrushEdge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(BrushVertex* vertex0 READ vertex0 WRITE setVertex0 NOTIFY vertex0Changed)
    Q_PROPERTY(BrushVertex* vertex1 READ vertex1 WRITE setVertex1 NOTIFY vertex1Changed)
public:
    explicit BrushEdge(QObject *parent = 0);
    explicit BrushEdge(BrushVertex* v0, BrushVertex* v1, QObject* parent = 0);

    BrushVertex* vertex0() const;
    void setVertex0(BrushVertex* v);

    BrushVertex* vertex1() const;
    void setVertex1(BrushVertex* v);

    bool isValid() const;

    // Two edges are equal if they both reference the same pair of vertices.
    // The order does not matter.
    bool operator==(const BrushEdge &other) const;

signals:
    void vertex0Changed(BrushVertex*);
    void vertex1Changed(BrushVertex*);

public slots:

private:
    QPointer<BrushVertex> m_pVertex0;
    QPointer<BrushVertex> m_pVertex1;
};

MODEL_END_NAMESPACE

#endif // BRUSHEDGE_H
