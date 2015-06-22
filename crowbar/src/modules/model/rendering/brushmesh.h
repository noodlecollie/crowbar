#ifndef BRUSHMESH_H
#define BRUSHMESH_H

#include "model_global.h"
#include <Qt3DRenderer/QAbstractMesh>

MODEL_BEGIN_NAMESPACE

class Brush;

class MODELSHARED_EXPORT BrushMesh : public Qt3D::QAbstractMesh
{
    Q_OBJECT
    Q_PROPERTY(Brush* brush READ brush WRITE setBrush NOTIFY brushChanged)
public:
    explicit BrushMesh(QNode *parent = 0);
    
    Qt3D::QAbstractMeshFunctorPtr meshFunctor() const Q_DECL_OVERRIDE;
    
    Brush* brush() const;
    void setBrush(Brush* b);
    
signals:
    void brushChanged(Brush*);
    
protected:
    void copy(const QNode *ref) Q_DECL_OVERRIDE;
    
private:
    QT3D_CLONEABLE(MODEL_NAMESPACE::BrushMesh)
    Brush* m_pBrush;
};

MODEL_END_NAMESPACE

#endif // BRUSHMESH_H
