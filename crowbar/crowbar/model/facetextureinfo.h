#ifndef FACETEXTUREINFO_H
#define FACETEXTUREINFO_H

#include <QObject>
#include "model_global.h"
#include <QGLTexture2D>
#include <QPointer>
#include <QVector2D>
#include <QVector3D>
#include <QSize>
#include "model_util.h"
#include "mapface.h"
#include <QPlane3D>

/* Some notes on this:
 * - Textures have their own co-ordinate system (u,v) on a plane in 3D space.
 * - The plane can be defined in:
 *   - Object space - relative to the brush
 *   - Parent space - relative to the brush's parent.
 *   - World space - relative to the world.
 * - The plane is defined by the U and V vectors in the chosen space.
 * - Additionally, a point in plane space is specified as the plane origin.
 *   The plane itself always goes through the origin of the parent co-ordinate system
 *   as only its orientation matters when projecting vertices onto it.
 * - Texture scaling scales the U and V vectors of the plane. The length of each
 *   vector defines the number of world units per texel in that axis.
 * - Texture rotation rotates the U and V vectors of the plane in 3D space,
 *   thereby changing the orientation of the plane.
 * - Texture translation translates the origin point on the plane, in texels, in the
 *   opposite direction to the translation.
 * - When calculating an eventual texture projection, translation is performed first, then scaling, then rotation.
 * - If face alignment flag is set, the texture plane is defined in object space and the origin of the space is
 *   defined to be the location of the centroid of the current face. The X and Y axes are defined as the two
 *   orthogonal axes to the axis closest to the face's normal.
 */

/* How texture planes work:
 * The U and V axes are specified as 3D vectors in the parent co-ordinate system.
 * A length of 1 unit in each of these directions (ie. u.length() and v.length()) corresponds to one texel.
 */

MODEL_BEGIN_NAMESPACE

class MapFace;

class MODELSHARED_EXPORT FaceTextureInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGLTexture2D* texture READ texture WRITE setTexture NOTIFY textureChanged)
    Q_PROPERTY(QVector2D translation READ translation WRITE setTranslation NOTIFY translationChanged)
    Q_PROPERTY(QVector2D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(float rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(QVector3D textureUAxis READ textureUAxis WRITE setTextureUAxis NOTIFY textureUAxisChanged)
    Q_PROPERTY(QVector3D textureVAxis READ textureVAxis WRITE setTextureVAxis NOTIFY textureVAxisChanged)
    Q_PROPERTY(float lightmapScale READ lightmapScale WRITE setLightmapScale NOTIFY lightmapScaleChanged)
    Q_PROPERTY(int smoothingGroups READ smoothingGroups WRITE setSmoothingGroups NOTIFY smoothingGroupsChanged)
    Q_PROPERTY(TextureAxisSpace axisSpace READ axisSpace WRITE setAxisSpace NOTIFY axisSpaceChanged)
    Q_PROPERTY(bool alignToFace READ alignToFace WRITE setAlignToFace NOTIFY alignToFaceChanged)
public:
    enum TextureAxisSpace
    {
        ObjectSpace,
        ParentSpace,
        WorldSpace
    };

    explicit FaceTextureInfo(MapFace *face = 0);

    inline MapFace* parentFace() const
    {
        return qobject_cast<MapFace*>(parent());
    }

    QGLTexture2D* texture() const;
    void setTexture(QGLTexture2D* tex);
    bool isValid() const;

    QVector2D translation() const;
    void setTranslation(const QVector2D &vec);

    QVector2D scale() const;
    void setScale(const QVector2D &vec);

    float rotation() const;
    void setRotation(float rot);

    // If both UV axes are 0, the axes are automatically determined
    // depending on the direction of the face normal.
    QVector3D textureUAxis() const;
    void setTextureUAxis(const QVector3D &vec);

    QVector3D textureVAxis() const;
    void setTextureVAxis(const QVector3D &vec);
    
    QVector3D texturePlaneNormal() const;
    bool isTexturePlaneValid() const;

    QPlane3D toPlane3D() const;

    float lightmapScale() const;
    void setLightmapScale(float scl);

    int smoothingGroups() const;
    void setSmoothingGroups(int groups);

    TextureAxisSpace axisSpace() const;
    void setAxisSpace(TextureAxisSpace space);

    bool alignToFace() const;
    void setAlignToFace(bool enabled);
    
    class TexturePlane
    {
    public:
        TexturePlane() : uAxis(QVector3D(0,0,0)), vAxis(QVector3D(0,0,0)), origin(QVector3D(0,0,0)), textureSize(QSize())
        {
        }
        
        TexturePlane(const QSize &texSize, const QVector3D &u, const QVector3D &v, const QVector3D &o = QVector3D()) : uAxis(u), vAxis(v), origin(o), textureSize(texSize)
        {
        }
        
        inline QVector3D normal() const { return QVector3D::normal(uAxis, vAxis); }
        inline QVector2D scale() const { return QVector2D(uAxis.length(), vAxis.length()); }
        
        QVector3D uAxis;    // Length is parent units per texel.
        QVector3D vAxis;    // Length is parent units per texel.
        QVector3D origin;   // Position of origin in parent co-ordinates.
        QSize textureSize;  // Dimensions of texture.
    };
    
    TexturePlane texturePlane() const;
    
    void autoGenerateTextureAxes(const QVector3D &normal);

signals:
    void textureChanged(QGLTexture2D*);
    void translationChanged(const QVector2D&);
    void scaleChanged(const QVector2D&);
    void rotationChanged(float);
    void textureUAxisChanged(const QVector3D&);
    void textureVAxisChanged(const QVector3D&);
    void lightmapScaleChanged(float);
    void smoothingGroupsChanged(int);
    void axisSpaceChanged(TextureAxisSpace);
    void alignToFaceChanged(bool);
    void dataChanged();

public slots:

private:
    void init();

    QPointer<QGLTexture2D>  m_pTexture;
    QVector2D               m_vecTranslaton;
    QVector2D               m_vecScale;
    float                   m_flRotation;
    QVector3D               m_vecUAxis;
    QVector3D               m_vecVAxis;
    float                   m_flLightmapScale;
    int                     m_iSmoothingGroups;
    TextureAxisSpace        m_iAxisSpace;
    bool                    m_bAlignToFace;
};

MODEL_END_NAMESPACE

#endif // FACETEXTUREINFO_H
