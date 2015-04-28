#include "tooleditface.h"
#include "mapview.h"
#include "mapdocument.h"
#include <QtDebug>
#include "mapbrush.h"

using namespace MODEL_NAMESPACE;

TOOLS_BEGIN_NAMESPACE

ToolEditFace::ToolEditFace(QObject *parent) :
    BaseTool(parent)
{
}

ToolEditFace::~ToolEditFace()
{
}

void ToolEditFace::processDragBegin(const QPoint &)
{
    MapFace* oldFace = m_pSelectedFace.data();

    // Get the face below the cursor.
    m_pSelectedFace = activeView()->faceAt(dragBegin());
    if ( oldFace && oldFace != m_pSelectedFace ) oldFace->setSelected(false);
    if ( !m_pSelectedFace.isNull() ) m_pSelectedFace->setSelected(true);
    emit faceSelected(m_pSelectedFace.data());
}

void ToolEditFace::processRightDragBegin(const QPoint &)
{
    // Get the face below the cursor.
    MapFace* face = activeView()->faceAt(rightDragBegin());
    
    // Set its texture.
    if ( face )
    {
        face->textureInfo()->setTexture(document()->currentTextureImage());
    }
}

void ToolEditFace::processDragMove(const QPoint &)
{
}

void ToolEditFace::processDragEnd()
{
}

void ToolEditFace::processActivate()
{
}

void ToolEditFace::processDeactivate()
{
    if ( !m_pSelectedFace.isNull() ) m_pSelectedFace->setSelected(false);
    m_pSelectedFace = NULL;
    emit faceSelected(m_pSelectedFace.data());
}

QString ToolEditFace::toolName() const
{
    return "Edit Face";
}

void ToolEditFace::applyCurrentTextureToAllSelectedFaces()
{
    if ( !document() || document()->selectedSet().count() < 1 ) return;
    
    QGLTexture2D* tex = document()->currentTextureImage();
    
    for ( MapComponentSet::Set::iterator it = document()->selectedSet().begin();
          it != document()->selectedSet().end();
          ++it )
    {
        MapBrush* brush = qobject_cast<MapBrush*>(*it);
        if ( !brush ) continue;
        
        for ( int i = 0; i < brush->faces().count(); i++ )
        {
            brush->faces(i)->textureInfo()->setTexture(tex);
        }
    }
}

TOOLS_END_NAMESPACE
