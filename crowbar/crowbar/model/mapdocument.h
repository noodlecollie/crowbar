#ifndef MAPDOCUMENT_H
#define MAPDOCUMENT_H

#include <QObject>
#include "model_global.h"
#include "mapworld.h"
#include <QList>
#include "mapcomponentset.h"
#include <QHash>
#include <QPointer>

// Not working right now.
//#define FAST_COMPONENT_HANDLING

class QGLLightParameters;
class QGLTexture2D;

MODEL_BEGIN_NAMESPACE

class MapBrush;
class MapLightBase;

class MODELSHARED_EXPORT MapDocument : public QObject
{
    Q_OBJECT

    typedef void (MapDocument::*ComponentHandler)(MapComponent*);
    typedef QHash<QString, ComponentHandler> ComponentHandlerTable;
    friend class MapComponent;
    
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(int gridPower READ gridPower WRITE setGridPower NOTIFY gridPowerChanged)
public:
    explicit MapDocument(QObject *parent = 0);
    ~MapDocument();
    
    QString displayName() const;
    void setDisplayName(const QString &name);
    inline void clearDisplayName() { setDisplayName(QString()); }

    MapWorld* world();
    const MapWorld* world() const;

    const QList<MapBrush*>& brushes() const;
    MapBrush* brushes(int index) const;
    
    MapComponentSet& selectedSet();
    const MapComponentSet& selectedSet() const;
    
    const QList<MapLightBase*>& lights() const;
    MapLightBase* lights(int index) const;
    QList<const QGLLightParameters*> lightParameters() const;

    int gridPower() const;
    void setGridPower(int power);
    inline void increaseGridPower() { setGridPower(gridPower() + 1); }
    inline void decreaseGridPower() { setGridPower(gridPower() - 1); }
    
    QGLTexture2D* errorTexture() const;
    void setErrorTexture(QGLTexture2D* tex);
    
    QGLTexture2D* texture(const QString &name);
    const QHash<QString, QGLTexture2D*>* textureTable() const;
    void setTextureTable(const QHash<QString, QGLTexture2D*>* table);
    int textureCount() const;

    QGLTexture2D* utilityTexture(const QString &name);
    const QHash<QString, QGLTexture2D*>* utilityTextureTable() const;
    void setUtilityTextureTable(const QHash<QString, QGLTexture2D*>* table);
    int utilityTextureCount() const;
    
    QString currentTexture() const;
    void setCurrentTexture(const QString &name);
    QGLTexture2D* currentTextureImage() const;

signals:
    void componentAdded(MapComponent*);
    void componentRemoved(MapComponent*);

    void renderUpdateHint();
    void displayNameChanged(const QString&);
    void gridPowerChanged(int);

public slots:

private slots:
    void componentAddedToSelectedSet(MapComponent* component);
    void componentRemovedFromSelectedSet(MapComponent* component);
    
    void handleAdd_MapBrush(MapComponent* c);
    void handleRemove_MapBrush(MapComponent* c);
    
    void handleAdd_MapLightBase(MapComponent* c);
    void handleRemove_MapLightBase(MapComponent* c);
    
private:
    void notifyComponentAdded(MapComponent* component);
    void notifyComponentRemoved(MapComponent* component);
    
    void removeFromComponentSets(MapComponent* component);
    
    MapWorld*   m_pWorld;

    QList<MapBrush*>        m_Brushes;
    QList<MapLightBase*>    m_Lights;
    MapComponentSet* m_pSelectedSet;

    QString m_szDisplayName;
    int m_iGridPower;
    
#ifdef FAST_COMPONENT_HANDLING
    QString classNameWithoutNamespace(const QString &classname);
#endif
    
    QPointer<QGLTexture2D> m_pErrorTexture;
    const QHash<QString, QGLTexture2D*>* m_pTextureTable;
    const QHash<QString, QGLTexture2D*>* m_pUtilityTextureTable;
    QString m_szCurrentTexture;
};

MODEL_END_NAMESPACE

#endif // MAPDOCUMENT_H
