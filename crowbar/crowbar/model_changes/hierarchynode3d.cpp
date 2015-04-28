#include "hierarchynode3d.h"
#include <QtMath>
#include <QtDebug>

MODEL_BEGIN_NAMESPACE

HierarchyNode3D::HierarchyNode3D(QObject* parent) : QObject(parent)
{
    m_iTransformationOrder = ScaleRotateTranslate;
    m_vecTranslation = QVector3D(0,0,0);
    m_vecRotation = QVector3D(0,0,0);
    m_vecScale = QVector3D(1,1,1);
    regenerateTransformationMatrix();
}

void HierarchyNode3D::setTransformationOrder(TransformationOrder order)
{
    if ( order != m_iTransformationOrder ) m_bMatricesStale = true;
    m_iTransformationOrder = order;
}

HierarchyNode3D::TransformationOrder HierarchyNode3D::transformationOrder() const
{
    return m_iTransformationOrder;
}

QVector3D HierarchyNode3D::translation() const
{
    return m_vecTranslation;
}

void HierarchyNode3D::setTranslation(const QVector3D &vec)
{
    if ( vec == m_vecTranslation ) return;
    m_bMatricesStale = true;
    m_vecTranslation = vec;
    emit translationChanged(m_vecTranslation);
}

QMatrix4x4 HierarchyNode3D::translationMatrix() const
{
    return QMatrix4x4(1.0f, 0.0f, 0.0f, m_vecTranslation.x(),
                      0.0f, 1.0f, 0.0f, m_vecTranslation.y(),
                      0.0f, 0.0f, 1.0f, m_vecTranslation.z(),
                      0.0f, 0.0f, 0.0f, 1.0f);
}

void HierarchyNode3D::setOrigin(const QVector3D &origin)
{
    if ( origin == m_vecOrigin ) return;
    emit originAboutToChange(origin);
    m_bMatricesStale = true;
    m_vecOrigin = origin;
    emit originChanged(m_vecOrigin);
}

QVector3D HierarchyNode3D::origin() const
{
    return m_vecOrigin;
}

float HierarchyNode3D::rotationX() const
{
    return m_vecRotation.x();
}

float HierarchyNode3D::rotationY() const
{
    return m_vecRotation.y();
}

float HierarchyNode3D::rotationZ() const
{
    return m_vecRotation.z();
}

void HierarchyNode3D::setRotationX(float radians)
{
    if ( radians == m_vecRotation.x() )return;
    m_bMatricesStale = true;
    m_vecRotation.setX(radians);
    emit rotationChanged(m_vecRotation);
}

void HierarchyNode3D::setRotationY(float radians)
{
    if ( radians == m_vecRotation.y() )return;
    m_bMatricesStale = true;
    m_vecRotation.setY(radians);
    emit rotationChanged(m_vecRotation);
}

void HierarchyNode3D::setRotationZ(float radians)
{
    if ( radians == m_vecRotation.z() )return;
    m_bMatricesStale = true;
    m_vecRotation.setZ(radians);
    emit rotationChanged(m_vecRotation);
}

QVector3D HierarchyNode3D::rotation() const
{
    return m_vecRotation;
}

void HierarchyNode3D::setRotation(const QVector3D &vec)
{
    if ( vec == m_vecRotation )return;
    m_bMatricesStale = true;
    m_vecRotation = vec;
    emit rotationChanged(m_vecRotation);
}

QMatrix4x4 HierarchyNode3D::rotationMatrix() const
{
    QMatrix4x4 x(   1.0f,   0.0f,               0.0f,                 0.0f,
                    0.0f,   qCos(rotationX()),  -qSin(rotationX()),   0.0f,
                    0.0f,   qSin(rotationX()),  qCos(rotationX()),    0.0f,
                    0.0f,   0.0f,               0.0f,                 1.0f);
    
    QMatrix4x4 y(   qCos(rotationY()),  0.0f,   qSin(rotationY()),    0.0f,
                    0.0f,               1.0f,   0.0f,                 0.0f,
                    -qSin(rotationY()), 0.0f,   qCos(rotationY()),    0.0f,
                    0.0f,               0.0f,   0.0f,                 1.0f);
    
    QMatrix4x4 z(   qCos(rotationZ()),   -qSin(rotationZ()),  0.0f,   0.0f,
                    qSin(rotationZ()),   qCos(rotationZ()),   0.0f,   0.0f,
                    0.0f,                0.0f,                1.0f,   0.0f,
                    0.0f,                0.0f,                0.0f,   1.0f);
    return x * y * z;
}

QVector3D HierarchyNode3D::scale() const
{
    return m_vecScale;
}

void HierarchyNode3D::setScale(const QVector3D &vec)
{
    if ( vec == m_vecScale)return;
    m_bMatricesStale = true;
    m_vecScale = vec;
    emit scaleChanged(m_vecScale);
}

QMatrix4x4 HierarchyNode3D::scaleMatrix() const
{
    QMatrix4x4 scl(m_vecScale.x(),   0.0f,           0.0f,           0.0f,
                   0.0f,             m_vecScale.y(), 0.0f,           0.0f,
                   0.0f,             0.0f,           m_vecScale.z(), 0.0f,
                   0.0f,             0.0f,           0.0f,           1.0f);
    return scl;
}

QMatrix4x4 HierarchyNode3D::transformationMatrix() const
{
    if ( m_bMatricesStale ) regenerateTransformationMatrix();
    return m_matTransformation;
}

QMatrix4x4 HierarchyNode3D::inverseTransformationMatrix() const
{
    if ( m_bMatricesStale ) regenerateTransformationMatrix();
    return m_matInverseTransformation;
}

void HierarchyNode3D::regenerateTransformationMatrix() const
{
    QMatrix4x4 first;
    if ( (m_iTransformationOrder & (Translation << First)) == Translation << First )
    {
        first = translationMatrix();
    }
    else if ( (m_iTransformationOrder & (Rotation << First)) == Rotation << First )
    {
        first = rotationMatrix();
    }
    else if ( (m_iTransformationOrder & (Scale << First)) == Scale << First )
    {
        first = scaleMatrix();
    }
    
    QMatrix4x4 second;
    if ( (m_iTransformationOrder & (Translation << Second)) == Translation << Second )
    {
        second = translationMatrix();
    }
    else if ( (m_iTransformationOrder & (Rotation << Second)) == Rotation << Second )
    {
        second = rotationMatrix();
    }
    else if ( (m_iTransformationOrder & (Scale << Second)) == Scale << Second )
    {
        second = scaleMatrix();
    }
    
    QMatrix4x4 third;
    if ( (m_iTransformationOrder & (Translation << Third)) == Translation << Third )
    {
        third = translationMatrix();
    }
    else if ( (m_iTransformationOrder & (Rotation << Third)) == Rotation << Third )
    {
        third = rotationMatrix();
    }
    else if ( (m_iTransformationOrder & (Scale << Third)) == Scale << Third )
    {
        third = scaleMatrix();
    }
    
    // It looks like we need to right-multiply rather than left-multiply...
    m_matTransformation = third * second * first;
    m_matInverseTransformation = m_matTransformation.inverted();
    
    m_bMatricesStale = false;
}

MODEL_END_NAMESPACE
