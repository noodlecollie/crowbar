#include "hierarchytransform_new.h"
#include "model_util.h"
#include <QtDebug>

MODEL_BEGIN_NAMESPACE

HierarchyTransform_New::HierarchyTransform_New(QObject *parent) :
    QObject(parent), m_vecTranslation(), m_angRotation(), m_vecScale(QVector3D(1,1,1)),
    m_vecTemporaryTranslation(), m_angTemporaryRotation(), m_vecTemporaryScale(QVector3D(1,1,1)),
    m_bUpdateTransform(false), m_matTransform(), m_bSetTemporaryTranslation(false),
    m_bSetTemporaryRotation(false), m_bSetTemporaryScale(false)
{
}

IHierarchyTransform* HierarchyTransform_New::ownerInterface() const
{
    return dynamic_cast<IHierarchyTransform*>(parent());
}

QVector3D HierarchyTransform_New::translation() const
{
    return m_vecTranslation;
}

void HierarchyTransform_New::setTranslation(const QVector3D &vec)
{
    if ( vec == m_vecTranslation ) return;

    m_vecTranslation = vec;
    invalidateTransform();
    emit translationChanged(m_vecTranslation);
    emit propertiesChanged();
    notifyChildrenParentPropertiesChanged(this);
}

QVector3D HierarchyTransform_New::scale() const
{
    return m_vecScale;
}

void HierarchyTransform_New::setScale(const QVector3D &vec)
{
    if ( m_vecScale == vec ) return;

    m_vecScale = vec;
    invalidateTransform();
    emit scaleChanged(m_vecScale);
    emit propertiesChanged();
    notifyChildrenParentPropertiesChanged(this);
}

EulerAngle HierarchyTransform_New::rotation() const
{
    return m_angRotation;
}

void HierarchyTransform_New::setRotation(const EulerAngle &rot)
{
    if ( rot == m_angRotation ) return;

    m_angRotation = rot;
    invalidateTransform();
    emit rotationChanged(m_angRotation);
    emit propertiesChanged();
    notifyChildrenParentPropertiesChanged(this);
}

void HierarchyTransform_New::invalidateTransform() const
{
    m_bUpdateTransform = true;
}

QMatrix4x4 HierarchyTransform_New::transformMatrix() const
{
    if ( m_bUpdateTransform ) recomputeTransform();

    return m_matTransform;
}

QVector3D HierarchyTransform_New::temporaryTranslation() const
{
    return m_vecTemporaryTranslation;
}

void HierarchyTransform_New::setTemporaryTranslation(const QVector3D &vec)
{
    if ( vec == m_vecTemporaryTranslation ) return;

    m_vecTemporaryTranslation = vec;
    m_bSetTemporaryTranslation = true;
    invalidateTransform();
    emit temporaryTranslationChanged(m_vecTemporaryTranslation);
    emit propertiesChanged();
    notifyChildrenParentPropertiesChanged(this);
}

bool HierarchyTransform_New::hasTemporaryTranslation() const
{
    return m_bSetTemporaryTranslation;
}

QVector3D HierarchyTransform_New::temporaryScale() const
{
    return m_vecTemporaryScale;
}

void HierarchyTransform_New::setTemporaryScale(const QVector3D &vec)
{
    if ( m_vecTemporaryScale == vec ) return;

    m_vecTemporaryScale = vec;
    m_bSetTemporaryScale = true;
    invalidateTransform();
    emit temporaryScaleChanged(m_vecTemporaryScale);
    emit propertiesChanged();
    notifyChildrenParentPropertiesChanged(this);
}

bool HierarchyTransform_New::hasTemporaryScale() const
{
    return m_bSetTemporaryScale;
}

EulerAngle HierarchyTransform_New::temporaryRotation() const
{
    return m_angTemporaryRotation;
}

void HierarchyTransform_New::setTemporaryRotation(const EulerAngle &rot)
{
    if ( rot == m_angTemporaryRotation ) return;

    m_angTemporaryRotation = rot;
    m_bSetTemporaryRotation = true;
    invalidateTransform();
    emit temporaryRotationChanged(m_angTemporaryRotation);
    emit propertiesChanged();
    notifyChildrenParentPropertiesChanged(this);
}

bool HierarchyTransform_New::hasTemporaryRotation() const
{
    return m_bSetTemporaryRotation;
}

void HierarchyTransform_New::mergeTemporaryTranslation()
{
    setTranslation(translation() + temporaryTranslation());
    clearTemporaryTranslation();
}

void HierarchyTransform_New::mergeTemporaryRotation()
{
    setRotation(rotation() + temporaryRotation());
    clearTemporaryRotation();
}

void HierarchyTransform_New::mergeTemporaryScale()
{
    setScale(scale() * temporaryScale());
    clearTemporaryScale();
}

void HierarchyTransform_New::mergeAllTemporaryProperties()
{
    mergeTemporaryTranslation();
    mergeTemporaryRotation();
    mergeTemporaryScale();
}

void HierarchyTransform_New::clearTemporaryTranslation()
{
    setTemporaryTranslation(QVector3D());
    m_bSetTemporaryTranslation = false;
}

void HierarchyTransform_New::clearTemporaryRotation()
{
    setTemporaryRotation(EulerAngle());
    m_bSetTemporaryRotation = false;
}

void HierarchyTransform_New::clearTemporaryScale()
{
    setTemporaryScale(QVector3D(1,1,1));
    m_bSetTemporaryScale = false;
}

QVector3D HierarchyTransform_New::overallTranslation() const
{
    return hasTemporaryTranslation() ? translation() + temporaryTranslation() : translation();
}

EulerAngle HierarchyTransform_New::overallRotation() const
{
    return hasTemporaryRotation() ? rotation() + temporaryRotation() : rotation();
}

QVector3D HierarchyTransform_New::overallScale() const
{
    return hasTemporaryScale() ? scale() * temporaryScale() : scale();
}

void HierarchyTransform_New::recomputeTransform() const
{
    QMatrix4x4 tr = Model_Util::translationMatrix(overallTranslation());
    QMatrix4x4 rt = overallRotation().rotationMatrix();
    QMatrix4x4 sc = Model_Util::scaleMatrix(overallScale());
    
    m_matTransform = tr * rt * sc;

    m_bUpdateTransform = false;
}

QMatrix4x4 HierarchyTransform_New::globalTransformMatrix() const
{
    if ( !ownerInterface() ) return transformMatrix();

    QMatrix4x4 mat = transformMatrix();
    IHierarchyTransform* p = ownerInterface()->transformParent();

    while ( p )
    {
        // Whoops, this was the wrong way around.
        mat = /*mat * */p->hierarchyTransform()->transformMatrix() * mat;
        p = p->transformParent();
    }

    return mat;
}

HierarchyTransform_New* HierarchyTransform_New::clone() const
{
    HierarchyTransform_New* tr = new HierarchyTransform_New();

    tr->setTranslation(translation());
    tr->setRotation(rotation());
    tr->setScale(scale());
    tr->setTemporaryTranslation(temporaryTranslation());
    tr->setTemporaryRotation(temporaryRotation());
    tr->setTemporaryScale(temporaryScale());

    return tr;
}

void HierarchyTransform_New::notifyChildrenParentPropertiesChanged(HierarchyTransform_New* transform) const
{
    if ( !ownerInterface() ) return;
        
    QList<IHierarchyTransform*> list = ownerInterface()->transformChildren();
    
    foreach ( IHierarchyTransform* tr, list )
    {
        tr->hierarchyTransform()->notifyParentPropertiesChanged(transform);
    }
}

void HierarchyTransform_New::notifyParentPropertiesChanged(HierarchyTransform_New *transform)
{
    Q_ASSERT(transform);
    emit parentPropertiesChanged(transform);
    notifyChildrenParentPropertiesChanged(transform);
}

MODEL_END_NAMESPACE
