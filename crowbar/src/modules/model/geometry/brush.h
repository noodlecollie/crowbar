#ifndef BRUSH_H
#define BRUSH_H

#include "model_global.h"
#include <Qt3DCore/QEntity>
#include <QVector>

/*
 * Add vertex - brush changes
 * Remove vertex - remove all face-to-vertex links for vertex, relevant faces change, brush changes
 * Add face - brush changes
 * Remove face - remove all vertex-to-face links for vertex, brush changes
 * Add face-to-vertex link - pair with a vertex-to-face link, vertex and face change, brush changes
 * Remove face-to-vertex link - pair with a vertex-to-face unlink, vertex and face change, brush changes
 * Change vertex: linked faces change, brush changes
 * Change face: brush changes.
 */

MODEL_BEGIN_NAMESPACE

class BrushVertex;
class BrushFace;

class MODELSHARED_EXPORT Brush : public Qt3D::QEntity
{
    Q_OBJECT
public:
    explicit Brush(QNode *parent = 0);

    void verticesAppend(BrushVertex* vertex);
    BrushVertex* verticesItemAt(int index) const;
    int verticesCount() const;
    void verticesClear();
    void verticesClean();

    void facesAppend(BrushFace* face);
    BrushFace* facesItemAt(int index) const;
    int facesCount() const;
    void facesClear();
    void facesClean();
    int totalFaceVertices() const;

signals:

public slots:

private:
    QVector<BrushVertex*> m_Vertices;
    QVector<BrushFace*> m_Faces;
};

MODEL_END_NAMESPACE

#endif // BRUSH_H
