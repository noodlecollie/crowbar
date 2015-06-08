#include "brushmesh.h"

MODEL_BEGIN_NAMESPACE

class BrushMeshFunctor;

void BrushMesh::copy(const Qt3D::QNode *ref)
{
    Qt3D::QAbstractMesh::copy(ref);
    const BrushMesh *mesh = static_cast<const BrushMesh*>(ref);
    
    // TODO later.
    Q_UNUSED(mesh);
}

BrushMesh::BrushMesh(Qt3D::QNode *parent) : Qt3D::QAbstractMesh(parent)
{
    // Initialise things before calling update.
    
    update();
}

Qt3D::QAbstractMeshFunctorPtr BrushMesh::meshFunctor() const
{
    return Qt3D::QAbstractMeshFunctorPtr(new BrushMeshFunctor(*this));
}

///////////////////////////////////////////////////////////
// BrushMeshFunctor
///////////////////////////////////////////////////////////

class BrushMeshFunctor : public Qt3D::QAbstractMeshFunctor
{
public:
    BrushMeshFunctor(const BrushMesh &brush)
            : Qt3D::QAbstractMeshFunctor()
    {
    }
    
    Qt3D::QMeshDataPtr operator()() Q_DECL_OVERRIDE
    {
        // TODO
        return Qt3D::QMeshDataPtr();
    }
    
    bool operator==(const QAbstractMeshFunctor &other) const Q_DECL_OVERRIDE
    {
        // TODO
        return true;
    }
    
private:
    
};

MODEL_END_NAMESPACE
