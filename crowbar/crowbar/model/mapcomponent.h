#ifndef MAPCOMPONENT_H
#define MAPCOMPONENT_H

#include <QObject>
#include "model_global.h"
#include "hierarchytransform_new.h"
#include "changesignal.h"
#include <QPointer>
#include <QBox3D>

// Helpers to automate MapComponent extension.
// Implement initialise()/dispose() as stand-ins for constructor/destructor.
#define MAPCOMPONENT_SUBCLASS(_classname, _parentclass)public: \
explicit _classname(MapComponent *parent = 0) : _parentclass(0) \
{ updateVirtualMetaInfo(); initialise(parent); } \
virtual ~_classname() \
{ baseCleanup(); dispose(); } \
private: \
void initialise(MapComponent* parent); \
void dispose();

MODEL_BEGIN_NAMESPACE

class MapDocument;

class MODELSHARED_EXPORT MapComponent : public QObject, public IHierarchyTransform
{
    Q_OBJECT
    Q_PROPERTY(bool hidden READ hidden WRITE setHidden NOTIFY hiddenStatusChanged)
    Q_PROPERTY(bool selected READ selected WRITE setSelected)
    Q_PROPERTY(MapDocument* document READ document)
    Q_PROPERTY(QString cachedClassName READ cachedClassName)
    Q_PROPERTY(bool billboard READ billboard WRITE setBillboard NOTIFY billboardStatusChanged)
    Q_PROPERTY(bool fixedBillboard READ fixedBillboard WRITE setFixedBillboard NOTIFY fixedBillboardChanged)

    friend class MapComponent;
    friend class MapSubComponent;
public:
    // Constructor, taking parent node as argument.
    // All nodes have a parent apart from the world (the root) - the parent of the world is the MapDocument,
    // but this is not a MapComponent.
    explicit MapComponent(MapComponent* parent = 0);

    virtual ~MapComponent();
    
    // IHierarchyTransform
    virtual IHierarchyTransform* transformParent() const;
    virtual QList<IHierarchyTransform*> transformChildren() const;
    virtual HierarchyTransform_New* hierarchyTransform();
    virtual const HierarchyTransform_New* hierarchyTransform() const;

    // Convenience function for returning the node's parent as a MapComponent.
    inline MapComponent* parentComponent() const
    {
        return qobject_cast<MapComponent*>(parent());
    }
    
    inline QList<MapComponent*> childComponents() const
    {
        return findChildren<MapComponent*>(QString(), Qt::FindDirectChildrenOnly);
    }

    // Sets the parent node and informs the MapDocument.
    // Use this in preference to setParent().
    void setParentComponent(MapComponent* parent);

    // MapDocument this node belongs to.
    // This is set on construction to be the same as that of the parent.
    // Once set, the document is the same for the lifetime of the component.
    // If the node needs to be added to a new document, clone the node instead.
    MapDocument* document() const;

    // Clones this MapComponent. Subclasses override this to deal with their own properties.
    // The cloned component does not have a parent or document set.
    // Other properties are copied over. The caller takes ownership of the component.
    virtual MapComponent* clone() const;

    // Hidden status of this component. If the component is hidden then all of its children
    // are hidden (but not vice-versa). The component is not hidden by default.
    bool hidden() const;
    void setHidden(bool hide);

    // Selected status of this component. Managed by the document.
    bool selected() const;
    void setSelected(bool selected);
    
    // If set, this component is a proxy for another component.
    MapComponent* proxyTarget() const;
    void setProxyTarget(MapComponent* component);
    bool isProxy() const;

    bool billboard() const;
    void setBillboard(bool enabled);
    
    bool fixedBillboard() const;
    void setFixedBillboard(bool enabled);
    
    QString cachedClassName() const;

    // These are local to the component's transform.
    virtual QBox3D localBoundingBox() const;
    virtual QVector3D localCentroid() const;
    virtual inline QBox3D globalBoundingBox() const { return localBoundingBox().transformed(hierarchyTransform()->globalTransformMatrix()); }
    virtual inline QVector3D globalCentroid() const { return localCentroid() * hierarchyTransform()->globalTransformMatrix(); }

#ifdef QT_DEBUG
    bool checkVirtualMetaInfo() const;
#endif

signals:
    void hiddenStatusChanged(bool);
    void renderDataChanged(ChangeId_t);
    void billboardStatusChanged(bool);
    void fixedBillboardChanged(bool);
    
public slots:
    void forceRenderDataUpdate();

protected:
    virtual void cloneProperties(MapComponent* c) const;
    void setDocument(MapDocument* document);
    void baseCleanup(); // Should always call this from the base class destructor.
    void updateVirtualMetaInfo();

protected slots:
    void sendRenderDataChange();
    void receiveComponentChange(ChangeId_t id);

private:
    MapDocument*            m_pDocument;
    HierarchyTransform_New* m_pTransform;
    bool                    m_bHidden;
    QPointer<MapComponent>  m_pProxyTarget;
    QString                 m_szCachedClassname;
    bool                    m_bCleanupOccurred;
    bool                    m_bIsBillboard;
    bool                    m_bFixedBillboard;
};

MODEL_END_NAMESPACE

#endif // MAPCOMPONENT_H
