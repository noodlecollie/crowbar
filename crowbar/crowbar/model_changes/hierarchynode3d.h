#ifndef HIERARCHYNODE3D_H
#define HIERARCHYNODE3D_H

#include "model_global.h"
#include <QList>
#include <QMatrix4x4>
#include <QVector3D>
#include <QtMath>
#include <QObject>

MODEL_BEGIN_NAMESPACE

class HierarchyNode3D;

class IHierarchyNode3D
{
public:
    virtual ~IHierarchyNode3D() {}

    virtual HierarchyNode3D& hierarchyNode() = 0;
    virtual const HierarchyNode3D& hierarchyNode() const = 0;
};

/**
 * @brief The HierarchyNode3D class defines an object's translation, rotation and scale relative to a parent object.
 * 
 * Hierarchy nodes allow a position, rotation and scaling offset to be assigned to an object. This essentially defines the co-ordinate
 * system of that object relative to its parent; if there is no parent, the co-ordinate system is relative to the global world co-ordinates.
 * If an object's co-ordinate system is changed, this modifies its position, as well as the positions of all of
 * its children, relative to any parents.
 * 
 * The position, rotation and scale values are used to generate a transformation matrix. Applying the matrix with OpenGL will convert the current
 * co-ordinate system from the parent to the object; applying the inverse matrix will convert back again.
 * 
 * Inside the class, data is stored as separate translation, rotation and scaling values. These can be obtained individually, obtained as 4x4 matrices,
 * or the entire transformation matrix can be obtained. Once the transformation matrix is retrieved, its result is cached for future retrievals;
 * the individual translation, rotation and scaling matrices are not cached.
 * 
 * Nodes do not own their children - the HierarchyNode3D class is designed to be used as a component in other classes, and so when when removing
 * children it may not be desirable for the children themselves to be destroyed. Nodes also cannot store null pointers.
 * 
 * The order in which the transformations are applied to generate the transformation matrix is specified by the TransformationOrder enum.
 * 
 * Hierarchy nodes can be used within either OpenGL or Hammer co-ordinate space, provided the space used is consistent throughout the entire node tree.
 */
class MODELSHARED_EXPORT HierarchyNode3D : public QObject
{
    Q_OBJECT
    friend class HierarchyNode3D;
    
    Q_PROPERTY(QVector3D origin READ origin WRITE setOrigin NOTIFY originChanged)
    Q_PROPERTY(QVector3D translation READ translation WRITE setTranslation NOTIFY translationChanged)
    Q_PROPERTY(QVector3D rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QMatrix4x4 transformationMatrix READ transformationMatrix)
    Q_PROPERTY(QMatrix4x4 inverseTransformationMatrix READ inverseTransformationMatrix)
    Q_PROPERTY(QMatrix4x4 translationMatrix READ translationMatrix)
    Q_PROPERTY(QMatrix4x4 rotationMatrix READ rotationMatrix)
    Q_PROPERTY(QMatrix4x4 scaleMatrix READ scaleMatrix)
    Q_PROPERTY(TransformationOrder transformationOrder READ transformationOrder)
private:
    /**
     * @brief Internally-used ID for transformations.
     */
    enum TransformationId
    {
        None = 0x0,
        Translation = 0x1,
        Rotation = 0x2,
        Scale = 0x4
    };
    
    /**
     * @brief Internally-used values for the order of transformations.
     */
    enum TransformationSlot
    {
        First = 0,
        Second = 3,
        Third = 6
    };
    
public:
    /**
     * @brief Defines the order in which the transformations are applied to the object.
     */
    enum TransformationOrder
    {
        TranslateRotateScale = (Translation << First) | (Rotation << Second) | (Scale << Third),    /// Translate, rotate, scale
        TranslateScaleRotate = (Translation << First) | (Scale << Second) | (Rotation << Third),    /// Translate, scale, rotate
        RotateScaleTranslate = (Rotation << First) | (Scale << Second) | (Translation << Third),    /// Rotate, scale, translate
        RotateTranslateScale = (Rotation << First) | (Translation << Second) | (Scale << Third),    /// Rotate, translate, scale
        ScaleTranslateRotate = (Scale << First) | (Translation << Second) | (Rotation << Third),    /// Scale, translate, rotate
        ScaleRotateTranslate = (Scale << First) | (Rotation << Second) | (Translation << Third)     /// Scale, rotate, translate
    };
    
    explicit HierarchyNode3D(QObject* parent = 0);
    
    /**
     * @brief Returns the enum entry corresponding to the current transformation order.
     * @return Transformation order.
     */
    TransformationOrder transformationOrder() const;
    
    /**
     * @brief Sets the transformation order.
     * @param order TransformationOrder entry representing the order.
     */
    void setTransformationOrder(TransformationOrder order);
    
    // Set the origin of the child object, in parent co-ordinates.
    QVector3D origin() const;
    void setOrigin(const QVector3D &origin);
    
    /**
     * @brief Returns the translation vector that represents this object's offset from its parent's origin.
     * @return Translation vector.
     */
    QVector3D translation() const;
    
    /**
     * @brief Sets the translation vector of this object from its parent's origin.
     * @param vec Translation.
     */
    void setTranslation(const QVector3D &vec);
    
    /**
     * @brief Returns the translation matrix for this object, generated from the translation() vector.
     * @return Translation matrix.
     */
    QMatrix4x4 translationMatrix() const;
    
    /**
     * @brief Returns this object's rotation in the X axis, in radians.
     * @return X rotation in radians.
     */
    float rotationX() const;
    inline float rotationDegX() const { return qRadiansToDegrees(rotationX()); }
    
    /**
     * @brief Returns this object's rotation in the Y axis, in radians.
     * @return Y rotation in radians.
     */
    float rotationY() const;
    inline float rotationDegY() const { return qRadiansToDegrees(rotationY()); }
    
    /**
     * @brief Returns this object's rotation in the Z axis, in radians.
     * @return Z rotation in radians.
     */
    float rotationZ() const;
    inline float rotationDegZ() const { return qRadiansToDegrees(rotationZ()); }
    
    /**
     * @brief Sets this object's rotation in the X axis, in radians.
     * @param radians X rotation in radians.
     */
    void setRotationX(float radians);
    inline void setRotationDegX(float degrees) { setRotationX(qDegreesToRadians(degrees)); }
    
    /**
     * @brief Sets this object's rotation in the y axis, in radians.
     * @param radians Y rotation in radians.
     */
    void setRotationY(float radians);
    inline void setRotationDegY(float degrees) { setRotationY(qDegreesToRadians(degrees)); }
    
    /**
     * @brief Sets this object's rotation in the Z axis, in radians.
     * @param radians Z rotation in radians.
     */
    void setRotationZ(float radians);
    inline void setRotationDegZ(float degrees) { setRotationZ(qDegreesToRadians(degrees)); }
    
    /**
     * @brief Returns the rotation around the X, Y and Z axes as a vector.
     * @return Vector containing rotations in radians.
     */
    QVector3D rotation() const;
    inline QVector3D rotationDeg() const { QVector3D rot = rotation(); return QVector3D(qRadiansToDegrees(rot.x()),qRadiansToDegrees(rot.y()),qRadiansToDegrees(rot.z())); }
    
    /**
     * @brief Sets the rotation around the X, Y and Z axes using a vector.
     * @param vec Rotations to set in radians.
     */
    void setRotation(const QVector3D &vec);
    inline void setRotationDeg(const QVector3D &vec) { setRotation(QVector3D(qDegreesToRadians(vec.x()),qDegreesToRadians(vec.y()),qDegreesToRadians(vec.z()))); }
    
    /**
     * @brief Returns the rotation matrix for this object, generated from the rotation() values.
     * @return Rotation matrix.
     */
    QMatrix4x4 rotationMatrix() const;
    
    /**
     * @brief Returns the scale of the object in each axis, as a vector.
     * @return Scale as a vector.
     */
    QVector3D scale() const;
    
    /**
     * @brief Sets the scale of the object in the X, Y and Z axes using the specified vector.
     * @param vec Vector containing the scaling values.
     */
    void setScale(const QVector3D &vec);
    
    /**
     * @brief Returns the scaling matrix for this object, generated from the scale() values.
     * @return 
     */
    QMatrix4x4 scaleMatrix() const;
    
    /**
     * @brief Returns the transformation matrix generated by concatenating the translation, rotation and scaling values in the order
     * specified by transformationOrder().
     * 
     * Once generated, the matrix returned is cached within the object until the transformation order or any of the transformation values
     * are changed.
     * @return Transformation matrix.
     */
    QMatrix4x4 transformationMatrix() const;
    
    /**
     * @brief Returns the inverse transformation matrix of transformationMatrix().
     * 
     * Once generated, the matrix returned is cached within the object until the transformation order or any of the transformation values
     * are changed.
     * @return Inverse transformation matrix.
     */
    QMatrix4x4 inverseTransformationMatrix() const;
    
signals:
    void originAboutToChange(const QVector3D &newOrigin);
    void originChanged(const QVector3D&);
    void translationChanged(const QVector3D&);
    void rotationChanged(const QVector3D&);
    void scaleChanged(const QVector3D&);
public slots:
    
private:
    QVector3D               m_vecOrigin;
    QVector3D               m_vecTranslation;           /// Translation vector.
    QVector3D               m_vecRotation;              /// Vector storing rotations around the X, Y and Z axes.
    QVector3D               m_vecScale;                 /// Vector storing scaling in the X, Y and Z axes.
    mutable QMatrix4x4      m_matTransformation;        /// The transformation matrix constructed by combining the above values.
    mutable QMatrix4x4      m_matInverseTransformation; /// The inverse matrix of the transformation matrix.
    mutable bool            m_bMatricesStale;           /// Whether the transformation matrices need updating.
    TransformationOrder     m_iTransformationOrder;     /// Order of transformations.
    
    void regenerateTransformationMatrix() const;
};

MODEL_END_NAMESPACE

#endif // HIERARCHYNODE3D_H
