#include "mapbrushbuilder.h"
#include "mapbrush.h"

using namespace MODEL_NAMESPACE;

TOOLS_BEGIN_NAMESPACE

MapBrushBuilder::MapBrushBuilder(QObject *parent) :
    QObject(parent)
{
}

MapBrush* MapBrushBuilder::axisAlignedBrush(const QBox3D &bounds, QGLTexture2D *texture)
{
    QVector3D centre = bounds.center();
    QVector3D halfDiag = centre - bounds.minimum();
    qDebug() << "Brush centre:" << centre << "Bounds:" << (centre - halfDiag) << "to" << (centre + halfDiag);
    MapBrush* brush = axisAlignedBrush(-halfDiag, halfDiag, texture);
    brush->hierarchyTransform()->setTranslation(centre);
    return brush;
}

MapBrush* MapBrushBuilder::axisAlignedBrush(const QVector3D &min, const QVector3D &max, QGLTexture2D *texture)
{
    MapBrush* brush = new MapBrush();
    MapBrush::VertexVector& vlist = brush->vertices();

    // Add vertices
    vlist.append(new MapVertex(min, brush));
    vlist.append(new MapVertex(QVector3D(max.x(),min.y(),min.z()), brush));
    vlist.append(new MapVertex(QVector3D(max.x(),max.y(),min.z()), brush));
    vlist.append(new MapVertex(QVector3D(min.x(),max.y(),min.z()), brush));

    vlist.append(new MapVertex(QVector3D(min.x(),min.y(),max.z()), brush));
    vlist.append(new MapVertex(QVector3D(max.x(),min.y(),max.z()), brush));
    vlist.append(new MapVertex(QVector3D(max)));
    vlist.append(new MapVertex(QVector3D(min.x(),max.y(),max.z()), brush));

    // Add edges
    brush->edges().append(new MapEdge(vlist.at(0), vlist.at(1), brush));
    brush->edges().append(new MapEdge(vlist.at(1), vlist.at(2), brush));
    brush->edges().append(new MapEdge(vlist.at(2), vlist.at(3), brush));
    brush->edges().append(new MapEdge(vlist.at(3), vlist.at(0), brush));

    brush->edges().append(new MapEdge(vlist.at(4), vlist.at(5), brush));
    brush->edges().append(new MapEdge(vlist.at(5), vlist.at(6), brush));
    brush->edges().append(new MapEdge(vlist.at(6), vlist.at(7), brush));
    brush->edges().append(new MapEdge(vlist.at(7), vlist.at(4), brush));

    brush->edges().append(new MapEdge(vlist.at(0), vlist.at(4), brush));
    brush->edges().append(new MapEdge(vlist.at(1), vlist.at(5), brush));
    brush->edges().append(new MapEdge(vlist.at(2), vlist.at(6), brush));
    brush->edges().append(new MapEdge(vlist.at(3), vlist.at(7), brush));

    // Add faces
    MapFace* face = new MapFace(brush);
    face->vertices().append(vlist.at(3));
    face->vertices().append(vlist.at(2));
    face->vertices().append(vlist.at(1));
    face->vertices().append(vlist.at(0));
    face->textureInfo()->setTexture(texture);
    brush->faces().append(face);
    face->autoGenerateTextureAxes();

    face = new MapFace(brush);
    face->vertices().append(vlist.at(4));
    face->vertices().append(vlist.at(5));
    face->vertices().append(vlist.at(6));
    face->vertices().append(vlist.at(7));
    face->textureInfo()->setTexture(texture);
    brush->faces().append(face);
    face->autoGenerateTextureAxes();

    face = new MapFace(brush);
    face->vertices().append(vlist.at(1));
    face->vertices().append(vlist.at(5));
    face->vertices().append(vlist.at(4));
    face->vertices().append(vlist.at(0));
    face->textureInfo()->setTexture(texture);
    brush->faces().append(face);
    face->autoGenerateTextureAxes();

    face = new MapFace(brush);
    face->vertices().append(vlist.at(2));
    face->vertices().append(vlist.at(6));
    face->vertices().append(vlist.at(5));
    face->vertices().append(vlist.at(1));
    face->textureInfo()->setTexture(texture);
    brush->faces().append(face);
    face->autoGenerateTextureAxes();

    face = new MapFace(brush);
    face->vertices().append(vlist.at(3));
    face->vertices().append(vlist.at(7));
    face->vertices().append(vlist.at(6));
    face->vertices().append(vlist.at(2));
    face->textureInfo()->setTexture(texture);
    brush->faces().append(face);
    face->autoGenerateTextureAxes();

    face = new MapFace(brush);
    face->vertices().append(vlist.at(0));
    face->vertices().append(vlist.at(4));
    face->vertices().append(vlist.at(7));
    face->vertices().append(vlist.at(3));
    face->textureInfo()->setTexture(texture);
    brush->faces().append(face);
    face->autoGenerateTextureAxes();

    return brush;
}

TOOLS_END_NAMESPACE
