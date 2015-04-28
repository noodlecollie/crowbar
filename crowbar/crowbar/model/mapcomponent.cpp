#include "mapcomponent.h"
#include "mapdocument.h"
#include "model_util.h"

MODEL_BEGIN_NAMESPACE

MapComponent::MapComponent(MapComponent *parent) : QObject(),
    m_pDocument(NULL), m_pTransform(new HierarchyTransform_New(this)), m_bHidden(false), m_pProxyTarget(),
    m_szCachedClassname(), m_bCleanupOccurred(false), m_bIsBillboard(false), m_bFixedBillboard(false)
{
    setParentComponent(parent);

    connect(m_pTransform, SIGNAL(propertiesChanged()), this, SLOT(sendRenderDataChange()));
}

MapComponent::~MapComponent()
{
    baseCleanup();
}

void MapComponent::baseCleanup()
{
    if ( m_bCleanupOccurred ) return;

    // Check that the first call to this function is by the derived class.
    Q_ASSERT(m_szCachedClassname == metaObject()->className());
    if ( m_szCachedClassname != metaObject()->className() )
    {
        qCritical().nospace() << "MapComponent::baseCleanup(): current classname " << metaObject()->className()
                              << " does not match cached classname"  << m_szCachedClassname
                              << ". Something is probably wrong.";
    }
    
    // For each child, set its parent to our current parent.
    MapComponent* p = parentComponent();
    if ( p )
    {
        QList<MapComponent*> children = childComponents();
        QMatrix4x4 globalToLocal = p->hierarchyTransform()->globalTransformMatrix().inverted();
        foreach ( MapComponent* c, children )
        {
            // Calculate the transformation matrix.
            QMatrix4x4 transform = globalToLocal * c->hierarchyTransform()->globalTransformMatrix();
            QVector3D translation;
            EulerAngle rotation;
            QVector3D scale;
            
            Model_Util::transformOrientation(transform, translation, rotation, scale);
            
            c->setParentComponent(p);
            c->hierarchyTransform()->setTranslation(translation);
            c->hierarchyTransform()->setRotation(rotation);
            c->hierarchyTransform()->setScale(scale);
        }
    }

    if ( document() )
    {
        document()->notifyComponentRemoved(this);
    }

    m_bCleanupOccurred = true;
}


void MapComponent::updateVirtualMetaInfo()
{
    m_szCachedClassname = metaObject()->className();
}

#ifdef QT_DEBUG
bool MapComponent::checkVirtualMetaInfo() const
{
    if ( metaObject()->className() != MapComponent::staticMetaObject.className() )
    {
        if ( m_szCachedClassname.isNull() )
        {
            qCritical() << "checkVirtualMetaInfo(): Cached classname is null."
                       << "MetaObject classname:" << metaObject()->className();
            return false;
        }

        if ( m_szCachedClassname != metaObject()->className() )
        {
            qCritical() << "checkVirtualMetaInfo(): Cached classname" << m_szCachedClassname
                        << "differs from reported metaObject classname" << metaObject()->className();
            return false;
        }
    }
    return true;
}
#endif

void MapComponent::setParentComponent(MapComponent *parent)
{
    // We shouldn't be re-parenting to a new document.
    Q_ASSERT(!document() || (parent && parent->document() == document()));

    if ( document() )
    {
        if ( !parent )
        {
            qWarning("WARNING: Unparenting component from document is not allowed!");
            return;
        }
        else if ( parent->document() != document() )
        {
            qWarning("WARNING: Changing component's parent document is not allowed!");
            return;
        }
    }

    setParent(parent);
    setDocument(parent ? parent->document() : NULL);
}

MapDocument* MapComponent::document() const
{
    return m_pDocument;
}

MapComponent* MapComponent::clone() const
{
    MapComponent* c = new MapComponent();
    cloneProperties(c);
    return c;
}

void MapComponent::cloneProperties(MapComponent *c) const
{
    c->setHidden(hidden());
    c->setSelected(selected());
    c->setProxyTarget(proxyTarget());

    delete c->m_pTransform;
    c->m_pTransform = hierarchyTransform()->clone();
    c->m_pTransform->setParent(c);
}

bool MapComponent::hidden() const
{
    return m_bHidden;
}

void MapComponent::setHidden(bool hide)
{
    if ( hide == m_bHidden ) return;

    m_bHidden = hide;
    emit hiddenStatusChanged(m_bHidden);
    sendRenderDataChange();
}

bool MapComponent::selected() const
{
    if ( !document() ) return false;
    
    return document()->selectedSet().contains(const_cast<MapComponent*>(this)); // FIXME: Const casting probably isn't the best way to deal with this.
}

void MapComponent::setSelected(bool selected)
{
    if ( !document() ) return;
    
    if ( selected )
    {
        document()->selectedSet().add(this);
    }
    else
    {
        document()->selectedSet().remove(this);
    }
}

void MapComponent::setDocument(MapDocument *document)
{
    if ( m_pDocument && document != m_pDocument )
    {
        qWarning("WARNING: Changing component's parent document is not allowed!");
        Q_ASSERT(false);
        return;
    }

    if ( document == m_pDocument ) return;
    
    // At this point we know we can only be gaining a document.
    m_pDocument = document;
    m_pDocument->notifyComponentAdded(this);

    // Set for any children we have.
    QList<MapComponent*> list = childComponents();
    foreach ( MapComponent* c, list )
    {
        c->setDocument(m_pDocument);
    }
}

void MapComponent::receiveComponentChange(ChangeId_t id)
{
    static ChangeId_t last_id = -1;

    if ( id != last_id )
    {
        last_id = id;
        emit renderDataChanged(id);
    }
}

void MapComponent::sendRenderDataChange()
{
    emit renderDataChanged(qrand());
}

void MapComponent::forceRenderDataUpdate()
{
    sendRenderDataChange();
}

MapComponent* MapComponent::proxyTarget() const
{
    return m_pProxyTarget.data();
}

void MapComponent::setProxyTarget(MapComponent *component)
{
    if ( component == m_pProxyTarget.data() ) return;
    
    if ( !m_pProxyTarget.isNull() )
    {
        disconnect(m_pProxyTarget.data(), &MapComponent::renderDataChanged, this, &MapComponent::sendRenderDataChange);
    }

    m_pProxyTarget = component;
    
    if ( !m_pProxyTarget.isNull() )
    {
        connect(m_pProxyTarget.data(), &MapComponent::renderDataChanged, this, &MapComponent::sendRenderDataChange);
    }
    
    sendRenderDataChange();
}

bool MapComponent::isProxy() const
{
    return !m_pProxyTarget.isNull();
}

QString MapComponent::cachedClassName() const
{
    return m_szCachedClassname;
}

QBox3D MapComponent::localBoundingBox() const
{
    return QBox3D();
}

QVector3D MapComponent::localCentroid() const
{
    return QVector3D();
}

IHierarchyTransform* MapComponent::transformParent() const
{
    return parentComponent();
}

QList<IHierarchyTransform*> MapComponent::transformChildren() const
{
    QList<MapComponent*> children = childComponents();
    QList<IHierarchyTransform*> list;
    
    foreach ( MapComponent* child, children )
    {
        list.append(child);
    }
    
    return list;
}

HierarchyTransform_New* MapComponent::hierarchyTransform()
{
    return m_pTransform;
}

const HierarchyTransform_New* MapComponent::hierarchyTransform() const
{
    return m_pTransform;
}

bool MapComponent::billboard() const
{
    return m_bIsBillboard;
}

void MapComponent::setBillboard(bool enabled)
{
    if ( enabled == m_bIsBillboard ) return;

    m_bIsBillboard = enabled;
    emit billboardStatusChanged(m_bIsBillboard);
    sendRenderDataChange();
}

bool MapComponent::fixedBillboard() const
{
    return m_bFixedBillboard;
}

void MapComponent::setFixedBillboard(bool enabled)
{
    if ( enabled == m_bFixedBillboard ) return;
    
    m_bFixedBillboard = enabled;
    emit fixedBillboardChanged(m_bFixedBillboard);
    sendRenderDataChange();
}

MODEL_END_NAMESPACE
