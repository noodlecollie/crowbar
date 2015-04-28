#ifndef MAPCOMPONENTSET_H
#define MAPCOMPONENTSET_H

#include <QObject>
#include <QSet>
#include "model_global.h"
#include <QList>
#include <QBox3D>

MODEL_BEGIN_NAMESPACE

class MapComponent;
class MapDocument;

// Holds a set of MapComponent pointers. Does not own the components.
// The components should exist for the duration of being part of the set.
class MODELSHARED_EXPORT MapComponentSet : public QObject
{
    Q_OBJECT
public:
    // Really we should use a QPointer but I'm not sure if it's wise to use them as keys.
    typedef QSet<MapComponent*> Set;
    
    explicit MapComponentSet(MapDocument *parent);
    ~MapComponentSet();
    
    MapDocument* document() const;
    
    void add(MapComponent* component);
    void add(const QList<MapComponent*> &list);
    bool contains(MapComponent* component) const;
    void remove(MapComponent* component);
    void remove(const QList<MapComponent*> &list);
    MapComponent* take(MapComponent* component);
    void clear();
    int count() const;
    
    Set::iterator begin();
    Set::const_iterator cbegin() const;
    Set::iterator end();
    Set::const_iterator cend() const;
    const Set& constData() const;
    
    QList<MapComponent*> items() const;

    QBox3D globalBoundingBox() const;
    QVector3D globalCentroid() const;
    
signals:
    // Paired - removed is guaranteed to be fired if added is fired for a component.
    void itemAdded(MapComponent*);
    void itemRemoved(MapComponent*);
    void itemChanged(MapComponent*);
    
public slots:
    
private slots:
    void flagRecomputeBounds() { m_bRecomputeBBox = true; }
    void handleItemChanged();

private:
    QBox3D computeBoundingBox() const;

    Set     m_ComponentTable;
    mutable bool    m_bRecomputeBBox;
    mutable QBox3D  m_Bounds;
};

MODEL_END_NAMESPACE

#endif // MAPCOMPONENTSET_H
