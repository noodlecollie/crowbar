#ifndef HIERARCHYTRANSFORM_NEW_H
#define HIERARCHYTRANSFORM_NEW_H

#include <QObject>
#include <QList>
#include "model_global.h"
#include <QMatrix4x4>
#include "eulerangle.h"
#include <QVector3D>

MODEL_BEGIN_NAMESPACE

class IHierarchyTransform;

class MODELSHARED_EXPORT HierarchyTransform_New : public QObject
{
    Q_OBJECT
    friend class HierarchyTransform_New;
    
    Q_PROPERTY(QVector3D translation READ translation WRITE setTranslation NOTIFY translationChanged)
    Q_PROPERTY(EulerAngle rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QVector3D temporaryTranslation READ temporaryTranslation WRITE setTemporaryTranslation NOTIFY temporaryTranslationChanged)
    Q_PROPERTY(EulerAngle temporaryRotation READ temporaryRotation WRITE setTemporaryRotation NOTIFY temporaryRotationChanged)
    Q_PROPERTY(QVector3D temporaryScale READ temporaryScale WRITE setTemporaryScale NOTIFY temporaryScaleChanged)
    Q_PROPERTY(bool hasTemporaryTranslation READ hasTemporaryTranslation)
    Q_PROPERTY(bool hasTemporaryRotation READ hasTemporaryRotation)
    Q_PROPERTY(bool hasTemporaryScale READ hasTemporaryScale)
public:
    explicit HierarchyTransform_New(QObject *parent = 0);

    IHierarchyTransform* ownerInterface() const;

    QMatrix4x4 transformMatrix() const;         // Transform from parent to local space, with origin translation applied.
    QMatrix4x4 globalTransformMatrix() const;   // Full transform from global to local space

    QVector3D translation() const;
    void setTranslation(const QVector3D &vec);

    QVector3D scale() const;
    void setScale(const QVector3D &vec);

    EulerAngle rotation() const;
    void setRotation(const EulerAngle &rot);

    QVector3D temporaryTranslation() const;
    void setTemporaryTranslation(const QVector3D &vec);
    void mergeTemporaryTranslation();
    bool hasTemporaryTranslation() const;
    void clearTemporaryTranslation();
    QVector3D overallTranslation() const;

    QVector3D temporaryScale() const;
    void setTemporaryScale(const QVector3D &vec);
    void mergeTemporaryScale();
    bool hasTemporaryScale() const;
    void clearTemporaryScale();
    QVector3D overallScale() const;

    EulerAngle temporaryRotation() const;
    void setTemporaryRotation(const EulerAngle &rot);
    void mergeTemporaryRotation();
    bool hasTemporaryRotation() const;
    void clearTemporaryRotation();
    EulerAngle overallRotation() const;

    void mergeAllTemporaryProperties();

    virtual HierarchyTransform_New* clone() const;

signals:
    void translationChanged(const QVector3D&);
    void scaleChanged(const QVector3D&);
    void rotationChanged(const EulerAngle&);

    void temporaryTranslationChanged(const QVector3D&);
    void temporaryScaleChanged(const QVector3D&);
    void temporaryRotationChanged(const EulerAngle&);
    
    void propertiesChanged();
    
    // Fired when a parent's properties change.
    // This could be any parent, not just direct parents.
    void parentPropertiesChanged(HierarchyTransform_New*);

public slots:

private slots:
    void notifyParentPropertiesChanged(HierarchyTransform_New* transform);
    void notifyChildrenParentPropertiesChanged(HierarchyTransform_New* transform) const;
    
private:
    void invalidateTransform() const;
    void recomputeTransform() const;

    QVector3D   m_vecTranslation;
    EulerAngle  m_angRotation;
    QVector3D   m_vecScale;

    QVector3D  m_vecTemporaryTranslation;
    EulerAngle m_angTemporaryRotation;
    QVector3D  m_vecTemporaryScale;

    mutable bool        m_bUpdateTransform;
    mutable QMatrix4x4  m_matTransform;
    
    bool    m_bSetTemporaryTranslation;
    bool    m_bSetTemporaryRotation;
    bool    m_bSetTemporaryScale;
};

// Interface for hierarchy transforms to use.
class IHierarchyTransform
{
public:
    virtual ~IHierarchyTransform() {}

    // Parent transformable object.
    virtual IHierarchyTransform* transformParent() const = 0;

    // Child transformable objects.
    virtual QList<IHierarchyTransform*> transformChildren() const = 0;

    // Hierarchy transform itself.
    virtual HierarchyTransform_New* hierarchyTransform() = 0;
    virtual const HierarchyTransform_New* hierarchyTransform() const = 0;
};

MODEL_END_NAMESPACE

#endif // HIERARCHYTRANSFORM_NEW_H
