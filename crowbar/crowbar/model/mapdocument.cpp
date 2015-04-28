#include "mapdocument.h"
#include "mapworld.h"
#include "mapcomponent.h"
#include "mapbrush.h"
#include <QtDebug>
#include <QMetaObject>
#include "maplightbase.h"
#include <QGLLightParameters>
#include <QGLTexture2D>

MODEL_BEGIN_NAMESPACE

typedef void(*ComponentHandler_t)(MapComponent*);

MapDocument::MapDocument(QObject *parent) :
    QObject(parent)
{
    m_iGridPower = 6;
    m_pWorld = new MapWorld(this);
    m_pWorld->setObjectName("World");
    m_pSelectedSet = new MapComponentSet(this);
    connect(m_pSelectedSet, SIGNAL(itemAdded(MapComponent*)), this, SLOT(componentAddedToSelectedSet(MapComponent*)));
    connect(m_pSelectedSet, SIGNAL(itemRemoved(MapComponent*)), this, SLOT(componentRemovedFromSelectedSet(MapComponent*)));
    connect(m_pSelectedSet, SIGNAL(itemChanged(MapComponent*)), this, SIGNAL(renderUpdateHint()));
}

MapDocument::~MapDocument()
{
    delete m_pWorld;
}

void MapDocument::notifyComponentAdded(MapComponent *component)
{
    Q_ASSERT(component);
    
#ifdef QT_DEBUG
    Q_ASSERT(component->checkVirtualMetaInfo());
#endif

#ifdef FAST_COMPONENT_HANDLING
    QString funcName = QString("handleAdd_%0").arg(classNameWithoutNamespace(component->metaObject()->className()));
    qDebug() << "Component added: calling function" << funcName;
    bool ret = metaObject()->invokeMethod(this, funcName.toLatin1(),
                                          Q_ARG(MapComponent*, component));
    Q_ASSERT(ret);
#else
    //if ( component->metaObject()->className() == MapBrush::staticMetaObject.className() )
    if ( qobject_cast<MapBrush*>(component) )
    {
        handleAdd_MapBrush(component);
    }
    else if ( qobject_cast<MapLightBase*>(component) )
    {
        handleAdd_MapLightBase(component);
    }
#endif

    emit componentAdded(component);
}

void MapDocument::notifyComponentRemoved(MapComponent *component)
{
    Q_ASSERT(component);

#ifdef FAST_COMPONENT_HANDLING
    QString funcName = QString("handleRemove_%0").arg(classNameWithoutNamespace(component->cachedClassName()));
    qDebug() << "Component removed: calling function" << funcName;
    bool ret = metaObject()->invokeMethod(this, funcName.toLatin1(),
                                          Q_ARG(MapComponent*, component));
    Q_ASSERT(ret);
#else
    //if ( component->metaObject()->className() == MapBrush::staticMetaObject.className() )
    if ( qobject_cast<MapBrush*>(component) )
    {
        handleRemove_MapBrush(component);
    }
    else if ( qobject_cast<MapLightBase*>(component) )
    {
        handleRemove_MapLightBase(component);
    }
#endif

    emit componentRemoved(component);
}

MapWorld* MapDocument::world()
{
    return m_pWorld;
}

const MapWorld* MapDocument::world() const
{
    return m_pWorld;
}

void MapDocument::handleAdd_MapBrush(MapComponent *c)
{
    MapBrush* brush = qobject_cast<MapBrush*>(c);
    Q_ASSERT(brush);
    m_Brushes.append(brush);
}

void MapDocument::handleRemove_MapBrush(MapComponent *c)
{
    MapBrush* brush = qobject_cast<MapBrush*>(c);
    Q_ASSERT(brush);
    removeFromComponentSets(brush);
    m_Brushes.removeOne(brush);
}

void MapDocument::handleAdd_MapLightBase(MapComponent *c)
{
    MapLightBase* light = qobject_cast<MapLightBase*>(c);
    Q_ASSERT(light);
    m_Lights.append(light);
}

void MapDocument::handleRemove_MapLightBase(MapComponent *c)
{
    MapLightBase* light = qobject_cast<MapLightBase*>(c);
    Q_ASSERT(light);
    m_Lights.removeOne(light);
}

void MapDocument::removeFromComponentSets(MapComponent *component)
{
    m_pSelectedSet->remove(component);
}

const QList<MapBrush*>& MapDocument::brushes() const
{
    return m_Brushes;
}

MapBrush* MapDocument::brushes(int index) const
{
    return m_Brushes.at(index);
}

const QList<MapLightBase*>& MapDocument::lights() const
{
    return m_Lights;
}

MapLightBase* MapDocument::lights(int index) const
{
    return m_Lights.at(index);
}

QList<const QGLLightParameters*> MapDocument::lightParameters() const
{
    QList<const QGLLightParameters*> list;
    
    for ( int i = 0; i < lights().count(); i++ )
    {
        const MapLightBase* light = lights(i);
        list.append(light->parameters());
    }
    
    return list;
}

MapComponentSet& MapDocument::selectedSet()
{
    return *m_pSelectedSet;
}

const MapComponentSet& MapDocument::selectedSet() const
{
    return *m_pSelectedSet;
}

void MapDocument::componentAddedToSelectedSet(MapComponent *component)
{
    if ( component )
    {
        component->forceRenderDataUpdate();
        renderUpdateHint();
    }
}

void MapDocument::componentRemovedFromSelectedSet(MapComponent *component)
{
    if ( component )
    {
        component->forceRenderDataUpdate();
        renderUpdateHint();
    }
}

#ifdef FAST_COMPONENT_HANDLING
QString MapDocument::classNameWithoutNamespace(const QString &classname)
{
    int index = classname.lastIndexOf(':');
    if ( index < 0 ) return classname;
    else if ( index == classname.length() - 1 ) return QString();
    return classname.mid(index+1);
}
#endif

QString MapDocument::displayName() const
{
    return m_szDisplayName;
}

void MapDocument::setDisplayName(const QString &name)
{
    if ( name == m_szDisplayName ) return;
    
    m_szDisplayName = name;
    emit displayNameChanged(m_szDisplayName);
}

int MapDocument::gridPower() const
{
    return m_iGridPower;
}

void MapDocument::setGridPower(int power)
{
    if ( power < 0 ) power = 0;
    else if ( power > 10 ) power = 10;

    if ( m_iGridPower == power ) return;

    m_iGridPower = power;
    emit gridPowerChanged(m_iGridPower);
}

QGLTexture2D* MapDocument::errorTexture() const
{
    return m_pErrorTexture.data();
}

void MapDocument::setErrorTexture(QGLTexture2D *tex)
{
    m_pErrorTexture = tex;
}

const QHash<QString, QGLTexture2D*>* MapDocument::textureTable() const
{
    return m_pTextureTable;
}

void MapDocument::setTextureTable(const QHash<QString, QGLTexture2D *> *table)
{
    m_pTextureTable = table;
}

QGLTexture2D* MapDocument::texture(const QString &name)
{
    return m_pTextureTable ? m_pTextureTable->value(name, NULL) : NULL;
}

int MapDocument::textureCount() const
{
    return m_pTextureTable ? m_pTextureTable->count() : 0;
}

const QHash<QString, QGLTexture2D*>* MapDocument::utilityTextureTable() const
{
    return m_pUtilityTextureTable;
}

void MapDocument::setUtilityTextureTable(const QHash<QString, QGLTexture2D *> *table)
{
    m_pUtilityTextureTable = table;
}

QGLTexture2D* MapDocument::utilityTexture(const QString &name)
{
    return m_pUtilityTextureTable ? m_pUtilityTextureTable->value(name, NULL) : NULL;
}

int MapDocument::utilityTextureCount() const
{
    return m_pUtilityTextureTable ? m_pUtilityTextureTable->count() : 0;
}

QString MapDocument::currentTexture() const
{
    return m_szCurrentTexture;
}

void MapDocument::setCurrentTexture(const QString &name)
{
    m_szCurrentTexture = name;
}

QGLTexture2D* MapDocument::currentTextureImage() const
{
    QGLTexture2D* tex = textureTable()->value(currentTexture(), NULL);
    return tex ? tex : errorTexture();
}

MODEL_END_NAMESPACE
