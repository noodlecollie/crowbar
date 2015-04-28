#ifndef MAPDOCUMENT_H
#define MAPDOCUMENT_H

#include "model_global.h"
#include <QObject>
#include "imaphandle.h"
#include "handleobjectlist.h"
#include "mapbrush.h"
#include "imaptitle.h"

// Titles of different sections within the document.
#define BRUSHES_TITLE   "brushes"
#define ENTITIES_TITLE  "entities"
#define HELPERS_TITLE   "helpers"
#define TEXTURES_TITLE  "textures"
#define LAYERS_TITLE    "layers"

MODEL_BEGIN_NAMESPACE

// Need a clearer ownership policy:
// When a brush is added to the document's brush list, the document then owns the brush.
// The easiest way to do this would be to parent the brush to the document - then when the
// document is destroyed, all brushes will automatically be destroyed too.
// When a brush is removed from the brush list, it is no longer owned by the document and so
// the parent is set to NULL. This is to allow brushes to be swapped between documents.
// However, when a brush is removed from a document a slot should be called in order for the
// brush to clean up any document-specific properties it holds that may not be valid if swapped
// to another document.
class MODELSHARED_EXPORT MapDocument : public QObject, public IMapTitle
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
public:
    typedef HandleObjectList<MapBrush> MapBrushList;
    
    // Takes a pointer to a tree model and a model index pointing to the parent node for this document.
    explicit MapDocument(QObject *parent = 0);
    
    ~MapDocument();
    
public: // IMapTitle
    virtual void setTitle(const QString &title);
    virtual QString title() const;
    
public: // Brushes:
    MapBrushList& brushes();
    const MapBrushList& brushes() const;
    
signals:
    void titleChanged(const QString&) const;

    void brushAdded(const MapBrush*);
    void brushRemoved(const MapBrush*);
public slots:

private slots:
    void handleBrushAdded(MapBrush* brush);
    void handleBrushRemoved(MapBrush* brush);
    
private:
    QString         m_szTitle;
    MapBrushList    m_BrushList;
};

MODEL_END_NAMESPACE

QDebug MODELSHARED_EXPORT operator<<(QDebug dbg, const MODEL_NAMESPACE::MapDocument &doc);

#endif // MAPDOCUMENT_H
