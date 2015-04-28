#include "applicationsettings.h"
#include "toolsetitem.h"
#include "toolselect.h"
#include "toolselectdockwidget.h"
#include "mapdocument.h"
#include "mapdocumentview.h"
#include "toolcreatebrush.h"
#include "toolcreatebrushdockwidget.h"
#include <QGLTexture2D>
#include <QApplication>
#include "tooleditface.h"
#include "tooleditfacedockwidget.h"
#include "tooledithierarchy.h"
#include "toolhierarchyeditdockwidget.h"

using namespace TOOLS_NAMESPACE;
using namespace MODEL_NAMESPACE;
using namespace VIEW_NAMESPACE;

ApplicationSettings& appSettings()
{
    static ApplicationSettings settings;
    return settings;
}

ApplicationSettings::ApplicationSettings(QObject *parent) :
    QObject(parent)
{
    setUpTextures();
    createTools();
}

ApplicationSettings::~ApplicationSettings()
{
    QList<QString> keys = m_ConstTextureTable.keys();
    foreach ( QString key, keys )
    {
        QGLTexture2D* tex = m_ConstTextureTable.value(key, NULL);
        delete tex;
    }
    m_ConstTextureTable.clear();
    
    keys = m_LoadedTextureTable.keys();
    foreach ( QString key, keys )
    {
        QGLTexture2D* tex = m_LoadedTextureTable.value(key, NULL);
        delete tex;
    }
    m_LoadedTextureTable.clear();
}

void ApplicationSettings::setUpTextures()
{
    QGLTexture2D* tex = new QGLTexture2D();
    tex->setImage(QImage(QString(":%0").arg(TEXTURE_MISSING)));
    tex->setHorizontalWrap(QGL::Repeat);
    tex->setVerticalWrap(QGL::Repeat);
    tex->setBindOptions(tex->bindOptions() ^ QGLTexture2D::InvertedYBindOption);
    tex->setObjectName(TEXTURE_MISSING);
    m_ConstTextureTable.insert(TEXTURE_MISSING, tex);

    tex = new QGLTexture2D();
    tex->setImage(QImage(QString(":%0").arg(TEXTURE_TPLANE)));
    tex->setHorizontalWrap(QGL::Repeat);
    tex->setVerticalWrap(QGL::Repeat);
    tex->setBindOptions(tex->bindOptions() ^ QGLTexture2D::InvertedYBindOption);
    tex->setObjectName(TEXTURE_TPLANE);
    m_ConstTextureTable.insert(TEXTURE_TPLANE, tex);
    Q_ASSERT(tex->hasAlphaChannel());

    tex = new QGLTexture2D();
    tex->setImage(QImage(QString(":%0").arg(TEXTURE_TPLANE_EDGE)));
    tex->setHorizontalWrap(QGL::Repeat);
    tex->setVerticalWrap(QGL::Repeat);
    tex->setBindOptions(tex->bindOptions() ^ QGLTexture2D::InvertedYBindOption);
    tex->setObjectName(TEXTURE_TPLANE_EDGE);
    m_ConstTextureTable.insert(TEXTURE_TPLANE_EDGE, tex);
    Q_ASSERT(tex->hasAlphaChannel());
    
    QDir dir(qApp->applicationDirPath() + QString("/textures"));
    if ( dir.exists() )
    {
        loadTexturesRecursive(dir, dir);
    }
}

void ApplicationSettings::notifyClosing()
{
    QList<QString> keys = m_ConstTextureTable.keys();
    foreach ( QString key, keys )
    {
        QGLTexture2D* tex = m_ConstTextureTable.value(key, NULL);
        if ( !tex ) continue;

        tex->cleanupResources();
        delete tex;
    }
    m_ConstTextureTable.clear();
    
    keys = m_LoadedTextureTable.keys();
    foreach ( QString key, keys )
    {
        QGLTexture2D* tex = m_LoadedTextureTable.value(key, NULL);
        if ( !tex ) continue;

        tex->cleanupResources();
        delete tex;
    }
    m_LoadedTextureTable.clear();
}

void ApplicationSettings::createTools()
{
    ToolSetItem* item = NULL;
    
    // Select tool.
    item = new ToolSetItem;
    ToolSelect* select = new ToolSelect(this);
    select->setDragBoxEnabled(true);
    ToolSelectDockWidget* selectDock = new ToolSelectDockWidget(select);
    item->setTool(select);
    item->setToolDockWidget(selectDock);
    m_ToolList.append(item);

    // Brush creation tool.
    item = new ToolSetItem;
    ToolCreateBrush* create = new ToolCreateBrush(this);
    ToolCreateBrushDockWidget* createDock = new ToolCreateBrushDockWidget(create);
    item->setTool(create);
    item->setToolDockWidget(createDock);
    m_ToolList.append(item);
    
    // Face selection tool.
    item = new ToolSetItem;
    ToolEditFace* editFace = new ToolEditFace(this);
    ToolEditFaceDockWidget* editFaceDock = new ToolEditFaceDockWidget(editFace);
    item->setTool(editFace);
    item->setToolDockWidget(editFaceDock);
    m_ToolList.append(item);
    
    // Hierarchy tool.
    item = new ToolSetItem;
    ToolEditHierarchy* editHierarchy = new ToolEditHierarchy(this);
    ToolHierarchyEditDockWidget* hierarchyDock = new ToolHierarchyEditDockWidget(editHierarchy);
    item->setTool(editHierarchy);
    item->setToolDockWidget(hierarchyDock);
    m_ToolList.append(item);
}

ToolSetItem* ApplicationSettings::toolSetItem(int index) const
{
    if ( index < 0 || index >= m_ToolList.count() ) return NULL;
    return m_ToolList.at(index);
}

ToolSetItem* ApplicationSettings::toolSetItem(const QString &toolName) const
{
    foreach ( ToolSetItem* item, m_ToolList )
    {
        if ( item->tool()->toolName() == toolName ) return item;
    }
    
    return NULL;
}

int ApplicationSettings::toolSetCount() const
{
    return m_ToolList.count();
}

int ApplicationSettings::documentCount() const
{
    return m_DocumentData.count();
}

MapDocument* ApplicationSettings::document(int index)
{
    if ( index < 0 || index >= m_DocumentData.count() ) return NULL;
    return m_DocumentData.at(index).document;
}

MapDocumentView* ApplicationSettings::documentView(int index)
{
    if ( index < 0 || index >= m_DocumentData.count() ) return NULL;
    return m_DocumentData.at(index).documentView;
}

MapDocument* ApplicationSettings::createDocument()
{
    MapDocument* doc = new MapDocument(this);
    doc->setErrorTexture(m_ConstTextureTable.value(TEXTURE_MISSING, NULL));
    doc->setTextureTable(&m_LoadedTextureTable);
    doc->setUtilityTextureTable(&m_ConstTextureTable);
    
    MapDocumentView* docView = new MapDocumentView(doc, this);
    
    MapDocumentData data;
    data.document = doc;
    data.documentView = docView;
    m_DocumentData.append(data);
    
    emit documentCreated(doc, docView);
    return doc;
}

void ApplicationSettings::removeDocument(int index)
{
    if ( index < 0 || index >= m_DocumentData.count() ) return;
    
    MapDocumentData data = m_DocumentData.takeAt(index);
    emit documentRemoved(data.document, data.documentView);
    delete data.documentView;
    delete data.document;
}

int ApplicationSettings::documentIndex(MapDocument *doc)
{
    if ( !doc ) return -1;
    
    for ( int i = 0; i < m_DocumentData.count(); i++ )
    {
        if ( m_DocumentData.at(i).document == doc ) return i; 
    }
    
    return -1;
}

int ApplicationSettings::toolSetIndex(const QString &toolName)
{
    for ( int i = 0; i < m_ToolList.count(); i++ )
    {
        if ( m_ToolList.at(i)->tool()->toolName() == toolName ) return i;
    }
    
    return -1;
}

QString ApplicationSettings::filePath(int index)
{
    return m_DocumentData.at(index).filepath;
}

void ApplicationSettings::setFilePath(int index, const QString &path)
{
    m_DocumentData[index].filepath = path;
}

QGLTexture2D* ApplicationSettings::loadedTexture(const QString &name)
{
    return m_LoadedTextureTable.value(name, NULL);
}

const QHash<QString, QGLTexture2D*>& ApplicationSettings::loadedTextures() const
{
    return m_LoadedTextureTable;
}

void ApplicationSettings::loadTexturesRecursive(const QDir &directory, const QDir &initialDirectory)
{
    // Search for all textures in the directory.
    QDir dir(directory);
    
    QStringList nameFilters;
    nameFilters << "*.png";
    dir.setNameFilters(nameFilters);
    dir.setFilter(QDir::Files | QDir::Readable);
    
    QFileInfoList textures = dir.entryInfoList();
    
    // Add the textures to the table.
    foreach ( QFileInfo info, textures )
    {
        QString path = info.canonicalFilePath();
        QImage image(path);
        
        path.remove(initialDirectory.canonicalPath());
        path.remove(QString(".") + info.completeSuffix());
        QGLTexture2D* tex = new QGLTexture2D();
        tex->setImage(image);
        tex->setHorizontalWrap(QGL::Repeat);
        tex->setVerticalWrap(QGL::Repeat);
        tex->setBindOptions(tex->bindOptions() ^ QGLTexture2D::InvertedYBindOption);
        tex->setObjectName(path);
        m_LoadedTextureTable.insert(path, tex);
        qDebug() << "Added texture" << path << "to table.";
    }
    
    // Search recursively.
    dir.setNameFilters(QStringList());
    dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    
    QFileInfoList newDirs = dir.entryInfoList();
    
    foreach ( QFileInfo info, newDirs )
    {
        dir.cd(info.canonicalFilePath());
        loadTexturesRecursive(dir, initialDirectory);
        dir.cdUp();
    }
}
