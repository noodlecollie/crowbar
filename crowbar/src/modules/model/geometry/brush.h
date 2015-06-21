#ifndef BRUSH_H
#define BRUSH_H

#include "model_global.h"
#include "changenotifier.h"
#include <QSet>

MODEL_BEGIN_NAMESPACE

struct VertexPrivate;
struct FacePrivate;

class BrushVertexReference;

class MODELSHARED_EXPORT Brush : public ChangeNotifier
{
    Q_OBJECT
public:
    explicit Brush(QObject *parent = 0);

signals:

public slots:

private slots:

private:
    QSet<VertexPrivate*> m_Vertices;
    VertexPrivate* createVertex();
    void destroyVertex(VertexPrivate* v);

    QSet<FacePrivate*> m_Faces;
    FacePrivate* createFace();
    void destroyFace(FacePrivate* f);
};

MODEL_END_NAMESPACE

#endif // BRUSH_H
