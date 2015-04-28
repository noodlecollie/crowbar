#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QObject>
#include <QList>
#include "tools_global.h"
#include "model_global.h"
#include "view_global.h"
#include <QHash>
#include <QDir>

class QGLTexture2D;

TOOLS_BEGIN_NAMESPACE
class ToolSetItem;
TOOLS_END_NAMESPACE

MODEL_BEGIN_NAMESPACE
class MapDocument;
MODEL_END_NAMESPACE

VIEW_BEGIN_NAMESPACE
class MapDocumentView;
VIEW_END_NAMESPACE

#define TEXTURE_MISSING "/TEX_MISSING"
#define TEXTURE_DEBUG01 "/tools/measure_texture_coordinates"
#define TEXTURE_TPLANE "/texture_plane"
#define TEXTURE_TPLANE_EDGE "/texture_plane_edge"

class ApplicationSettings : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationSettings(QObject *parent = 0);
    ~ApplicationSettings();

    TOOLS_NAMESPACE::ToolSetItem* toolSetItem(int index) const;
    TOOLS_NAMESPACE::ToolSetItem* toolSetItem(const QString &toolName) const;
    int toolSetCount() const;
    int toolSetIndex(const QString &toolName);
    
    MODEL_NAMESPACE::MapDocument* createDocument();
    MODEL_NAMESPACE::MapDocument* document(int index);
    VIEW_NAMESPACE::MapDocumentView* documentView(int index);
    int documentIndex(MODEL_NAMESPACE::MapDocument* doc);
    int documentCount() const;
    void removeDocument(int index);

    QString filePath(int index);
    void setFilePath(int index, const QString &path);

    QGLTexture2D* constTexture(const QString &name);
    QGLTexture2D* loadedTexture(const QString &name);
    const QHash<QString, QGLTexture2D*>& loadedTextures() const;

signals:
    void documentCreated(MODEL_NAMESPACE::MapDocument*, VIEW_NAMESPACE::MapDocumentView*);
    void documentRemoved(MODEL_NAMESPACE::MapDocument*, VIEW_NAMESPACE::MapDocumentView*);

public slots:
    void notifyClosing();

private:
    void setUpTextures();
    void loadTexturesRecursive(const QDir &directory, const QDir &initialDirectory);

    struct MapDocumentData
    {
        MODEL_NAMESPACE::MapDocument* document;
        VIEW_NAMESPACE::MapDocumentView* documentView;
        QString filepath;
    };
    
    void createTools();
    
    QList<TOOLS_NAMESPACE::ToolSetItem*>    m_ToolList;
    QList<MapDocumentData> m_DocumentData;
    
    QHash<QString, QGLTexture2D*> m_ConstTextureTable;
    QHash<QString, QGLTexture2D*> m_LoadedTextureTable;
};

ApplicationSettings& appSettings();

#endif // APPLICATIONSETTINGS_H
