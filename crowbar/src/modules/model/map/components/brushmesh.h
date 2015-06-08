#ifndef BRUSHMESH_H
#define BRUSHMESH_H

#include "model_global.h"
#include <Qt3DRenderer/QAbstractMesh>

MODEL_BEGIN_NAMESPACE

class MODELSHARED_EXPORT BrushMesh : public Qt3D::QAbstractMesh
{
    Q_OBJECT
public:
    explicit BrushMesh(Qt3D::QNode *parent = 0);
    
    Qt3D::QAbstractMeshFunctorPtr meshFunctor() const Q_DECL_OVERRIDE;
    
signals:
    
public slots:
    
protected:
    void copy(const Qt3D::QNode *ref) Q_DECL_OVERRIDE;
    
private:
    QT3D_CLONEABLE(BrushMesh)
};

MODEL_END_NAMESPACE

#endif // BRUSHMESH_H
