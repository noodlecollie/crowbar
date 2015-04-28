#include "facetextureinfo.h"

MODEL_BEGIN_NAMESPACE

FaceTextureInfo::FaceTextureInfo(MapFace *face) :
    QObject(face)
{
    init();
}

void FaceTextureInfo::init()
{
    m_pTexture = NULL;
    m_vecTranslaton = QVector2D();
    m_vecScale = QVector2D(0.25,0.25);
    m_flRotation = 0.0f;
    m_flLightmapScale = 16.0f;
    m_iSmoothingGroups = 0;
    m_bAlignToFace = false;

    // Default to world, auto-determine U and V.
    m_iAxisSpace = WorldSpace;
    m_vecUAxis = QVector3D();
    m_vecVAxis = QVector3D();
}

QGLTexture2D* FaceTextureInfo::texture() const
{
    return m_pTexture.data();
}

void FaceTextureInfo::setTexture(QGLTexture2D *tex)
{
    if ( tex == m_pTexture.data() ) return;

    m_pTexture = tex;
    emit textureChanged(m_pTexture.data());
    emit dataChanged();
}

QVector2D FaceTextureInfo::translation() const
{
    return m_vecTranslaton;
}

void FaceTextureInfo::setTranslation(const QVector2D &vec)
{
    if ( m_vecTranslaton == vec ) return;

    m_vecTranslaton = vec;
    emit translationChanged(m_vecTranslaton);
    emit dataChanged();
}

QVector2D FaceTextureInfo::scale() const
{
    return m_vecScale;
}

void FaceTextureInfo::setScale(const QVector2D &vec)
{
    if ( m_vecScale == vec ) return;

    m_vecScale = vec;
    emit scaleChanged(m_vecScale);
    emit dataChanged();
}

float FaceTextureInfo::rotation() const
{
    return m_flRotation;
}

void FaceTextureInfo::setRotation(float rot)
{
    if ( m_flRotation == rot ) return;

    m_flRotation = rot;
    emit rotationChanged(m_flRotation);
    emit dataChanged();
}

QVector3D FaceTextureInfo::textureUAxis() const
{
    return m_vecUAxis;
}

void FaceTextureInfo::setTextureUAxis(const QVector3D &vec)
{
    if ( m_vecUAxis == vec ) return;

    // Remove me!
    Q_ASSERT(!vec.isNull());

    m_vecUAxis = vec;
    emit textureUAxisChanged(m_vecUAxis);
    emit dataChanged();
}

QVector3D FaceTextureInfo::textureVAxis() const
{
    return m_vecVAxis;
}

void FaceTextureInfo::setTextureVAxis(const QVector3D &vec)
{
    if ( m_vecVAxis == vec ) return;

    // Remove me!
    Q_ASSERT(!vec.isNull());

    m_vecVAxis = vec;
    emit textureVAxisChanged(m_vecVAxis);
    emit dataChanged();
}

float FaceTextureInfo::lightmapScale() const
{
    return m_flLightmapScale;
}

void FaceTextureInfo::setLightmapScale(float scl)
{
    if ( m_flLightmapScale == scl ) return;

    m_flLightmapScale = scl;
    emit lightmapScaleChanged(m_flLightmapScale);
    emit dataChanged();
}

int FaceTextureInfo::smoothingGroups() const
{
    return m_iSmoothingGroups;
}

void FaceTextureInfo::setSmoothingGroups(int groups)
{
    if ( m_iSmoothingGroups == groups ) return;

    m_iSmoothingGroups = groups;
    emit smoothingGroupsChanged(m_iSmoothingGroups);
    emit dataChanged();
}

FaceTextureInfo::TextureAxisSpace FaceTextureInfo::axisSpace() const
{
    return m_iAxisSpace;
}

void FaceTextureInfo::setAxisSpace(TextureAxisSpace space)
{
    if ( m_iAxisSpace == space ) return;

    m_iAxisSpace = space;
    emit axisSpaceChanged(m_iAxisSpace);
    emit dataChanged();
}

bool FaceTextureInfo::alignToFace() const
{
    return m_bAlignToFace;
}

void FaceTextureInfo::setAlignToFace(bool enabled)
{
    if ( m_bAlignToFace == enabled ) return;

    m_bAlignToFace = enabled;
    emit alignToFaceChanged(m_bAlignToFace);
    emit dataChanged();
}

bool FaceTextureInfo::isTexturePlaneValid() const
{
    return !texturePlaneNormal().isNull();
}

QVector3D FaceTextureInfo::texturePlaneNormal() const
{
    if ( textureUAxis().isNull() || textureVAxis().isNull() ) return QVector3D();
    return QVector3D::normal(textureUAxis(), textureVAxis());
}

FaceTextureInfo::TexturePlane FaceTextureInfo::texturePlane() const
{
    if ( m_pTexture.isNull() ) return TexturePlane();
    return TexturePlane(m_pTexture->size(), textureUAxis().normalized(), textureVAxis().normalized());
}

bool FaceTextureInfo::isValid() const
{
    return !m_pTexture.isNull();
}

void FaceTextureInfo::autoGenerateTextureAxes(const QVector3D &normal)
{
    QVector3D u, v;
    Model_Util::cheapTextureAxes(normal, u, v);

#ifdef QT_DEBUG
    if ( u.isNull() )
    {
        qWarning() << "autoGenerateTextureAxes(): U axis generated as null!";
    }

    if ( v.isNull() )
    {
        qWarning() << "autoGenerateTextureAxes(): V axis generated as null!";
    }
#endif

    setTextureUAxis(u);
    setTextureVAxis(v);
}

QPlane3D FaceTextureInfo::toPlane3D() const
{
    if ( textureUAxis().isNull() || textureVAxis().isNull() ) return QPlane3D();

    return QPlane3D(QVector3D(0,0,0), texturePlaneNormal());
}

MODEL_END_NAMESPACE
