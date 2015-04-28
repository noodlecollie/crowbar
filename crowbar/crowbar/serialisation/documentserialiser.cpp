#include "documentserialiser.h"
#include <QJsonObject>
#include "model_util.h"
#include "mapbrush.h"
#include <QJsonArray>
#include "keyvaluesparser.h"

using namespace MODEL_NAMESPACE;

SERIALISATION_BEGIN_NAMESPACE

DocumentSerialiser::DocumentSerialiser(QObject *parent) :
    QObject(parent)
{
}

void DocumentSerialiser::toVMF(const MapDocument *document, QByteArray &vmf)
{
    vmf.clear();
    if ( !document ) return;
    
    // Root object for the JSON document.
    QJsonObject root;
    
    // Add the version info.
    root.insert("versioninfo", QJsonValue(defaultVersionInfo()));
    
    // Add the view settings.
    root.insert("viewsettings", QJsonValue(defaultViewSettings()));
    
    // Add the camera settings.
    root.insert("cameras", QJsonValue(defaultCameras()));
    
    // Add the cordon settings.
    root.insert("cordon", QJsonValue(defaultCordons()));
    
    // Create a world object.
    QJsonObject world = emptyWorld();
    
    // Populate it with the brushes from the map.
    populateEmptyWorld(document, world);
    
    // Insert the world.
    root.insert("world", QJsonValue(world));

    // Put it all in a document.
    QJsonDocument doc;
    doc.setObject(root);

    // Serialise to VMF.
    KeyValuesParser parser;
    parser.jsonToKeyValues(doc, vmf);
}

void DocumentSerialiser::populateEmptyWorld(const MapDocument *document, QJsonObject &world)
{
    int currentFaceId = 1;
    int currentBrushId = 1;
    
    QJsonArray array;
    for ( int i = 0; i < document->brushes().count(); i++ )
    {
        array.append(QJsonValue(convertBrush(document->brushes(i), currentBrushId, currentFaceId)));
        currentBrushId++;
    }

    world.insert("solid", array);
}

QJsonObject DocumentSerialiser::defaultVersionInfo()
{
//    versioninfo
//    {
//        "editorversion" "400"   <- Value irrelevant
//        "editorbuild" "4940"    <- Value irrelevant
//        "mapversion" "36"       <- Value irrelevant
//        "formatversion" "100"   <- Value irrelevant
//        "prefab" "0"            <- Should be 0
//    }
    
    QJsonObject object;
    
    object.insert("editorversion", QJsonValue(QString("400")));
    object.insert("editorbuild", QJsonValue(QString("4940")));
    object.insert("mapversion", QJsonValue(QString("36")));
    object.insert("formatversion", QJsonValue(QString("100")));
    object.insert("prefab", QJsonValue(QString("0")));
    
    return object;
}

QJsonObject DocumentSerialiser::defaultViewSettings()
{
//    viewsettings    <- Don't really matter but should probably just be set to this
//    {
//        "bSnapToGrid" "1"
//        "bShowGrid" "1"
//        "bShowLogicalGrid" "0"
//        "nGridSpacing" "64"
//        "bShow3DGrid" "0"
//    }
    
    QJsonObject object;
    
    object.insert("bSnapToGrid", QJsonValue(QString("1")));
    object.insert("bShowGrid", QJsonValue(QString("1")));
    object.insert("bShowLogicalGrid", QJsonValue(QString("0")));
    object.insert("nGridSpacing", QJsonValue(QString("64")));
    object.insert("bShow3DGrid", QJsonValue(QString("1")));
    
    return object;
}

QJsonObject DocumentSerialiser::defaultCameras()
{
//    cameras <- No camera support right now
//    {
//        "activecamera" "-1"
//    }
    
    QJsonObject object;
    object.insert("activecamera", QJsonValue(QString("-1")));
    return object;
}

QJsonObject DocumentSerialiser::defaultCordons()
{
//    cordon  <- Again doesn't really matter but make sure active is 0
//    {
//        "mins" "(-1024 -1024 -1024)"
//        "maxs" "(1024 1024 1024)"
//        "active" "0"
//    }
    
    QJsonObject object;
    
    object.insert("mins", QJsonValue(QString("(-1024 -1024 -1024)")));
    object.insert("maxs", QJsonValue(QString("(1024 1024 1024)")));
    object.insert("active", QJsonValue(QString("0")));
    
    return object;
}

QJsonObject DocumentSerialiser::emptyWorld()
{
//    world
//    {
//        "id" "1"                    <- Should probably be 1. Solid IDs then increase from 2.
//        "mapversion" "1"            <- Should probably match that in the versioninfo
//        "classname" "worldspawn"    <- Should be worldspawn
//        "skyname" "sky_tf2_04"      <- Doesn't matter but should be set by the map eventually
//        "maxpropscreenwidth" "-1"   <- Should probably stay at -1
//        "detailvbsp" "detail_2fort.vbsp"                <- Doesn't matter but leave at HL2 default
//        "detailmaterial" "detail/detailsprites_2fort"   <- Doesn't matter but leave at HL2 default
//        "solid"
//        {
//            ....
//        }
//        ....
//    }
    
    QJsonObject object;
    
    object.insert("id", QJsonValue(QString("1")));
    object.insert("mapversion", QJsonValue(QString("1")));
    object.insert("classname", QJsonValue(QString("worldspawn")));
    object.insert("skyname", QJsonValue(QString("sky_day01_01")));
    object.insert("maxpropscreenwidth", QJsonValue(QString("-1")));
    object.insert("detailvbsp", QJsonValue(QString("detail.vbsp")));
    object.insert("detailmaterial", QJsonValue(QString("detail/detailsprites")));
    object.insert("solid", QJsonValue(QJsonArray()));

    return object;
}

QJsonObject DocumentSerialiser::convertFace(const MapFace *face, int id)
{
//    side
//    {
//        "id" "1"                                      <- Unique in world
//        "plane" "(0 128 64) (128 128 64) (128 0 64)"
//        "material" "BRICK/BRICKFLOOR001A"
//        "uaxis" "[1 0 0 0] 0.25"
//        "vaxis" "[0 -1 0 0] 0.25"
//        "rotation" "0"
//        "lightmapscale" "16"
//        "smoothing_groups" "0"                        <- Leave at 0 for now.
//    }
    
    QJsonObject object;
    
    object.insert("id", QJsonValue(QString("%0").arg(id)));
    object.insert("rotation", QJsonValue(QString("%0").arg(face->textureInfo()->rotation())));
    object.insert("lightmapscale", QJsonValue(QString("%0").arg(face->textureInfo()->lightmapScale())));
    object.insert("smoothing_groups", QJsonValue("0"));
    
    // Strip the preceding '/' from the texture name.
    QString texname = face->textureInfo()->texture()->objectName();
    if ( !texname.isEmpty() ) texname = texname.mid(1);
    object.insert("material", QJsonValue(texname));
    
    Q_ASSERT(face->parentComponent());
    QMatrix4x4 toGlobal = face->parentComponent()->hierarchyTransform()->globalTransformMatrix();
    int numVertices = face->vertices().constData().count();
    
    QVector3D v0 = 0 < numVertices ? toGlobal * face->vertices(0)->position() : QVector3D();
    QVector3D v1 = 1 < numVertices ? toGlobal * face->vertices(1)->position() : QVector3D();
    QVector3D v2 = 2 < numVertices ? toGlobal * face->vertices(2)->position() : QVector3D();
    
    QString sv0 = QString("(%0 %1 %2)").arg(v0.x()).arg(v0.y()).arg(v0.z());
    QString sv1 = QString("(%0 %1 %2)").arg(v1.x()).arg(v1.y()).arg(v1.z());
    QString sv2 = QString("(%0 %1 %2)").arg(v2.x()).arg(v2.y()).arg(v2.z());
    
    // Hammer either calculates the cross-product differently to Qt,
    // or wants the face normals pointing inwards.
    // Either way, we specify the vertices in 0-2-1 order to account for this.
    QString plane = sv0 + " " + sv2 + " " + sv1;
    object.insert("plane", QJsonValue(plane));
    
    QVector3D uAxis = face->textureInfo()->textureUAxis();
    QVector3D vAxis = face->textureInfo()->textureVAxis();
    QVector2D translation = face->textureInfo()->translation();
    QVector2D scale = face->textureInfo()->scale();
    
    // The rotation value is just aesthetical in Hammer - the actual texture axes must be rotated.
    // Do this here.
    QQuaternion texRot = QQuaternion::fromAxisAndAngle(face->textureInfo()->texturePlaneNormal(), face->textureInfo()->rotation());
    uAxis = texRot.rotatedVector(uAxis);
    vAxis = texRot.rotatedVector(vAxis);
    
    // FIXME: If the rotation in Hammer is defined by the orientation of the axes,
    // and our rotation is a value that's applied -on top of- the prescribed axes,
    // that means simply copying this value into the rotation field will only be accurate
    // if the axes were left at their defaults in Crowbar.
    // We need to work out exactly how Hammer calculates what value to export in the rotation field.
    // This isn't absolutely critical, since all it'll mean is the value in the rotation box
    // in the face edit dialogue will be wrong until the user manipulates it, but it would be
    // nice to get right.
    object.insert("rotation", QJsonValue(QString("%0").arg(face->textureInfo()->rotation())));
    
    // For some reason Hammer requires the V axis be negated here.
    // The translation value remains the same though.
    QString su = QString("[%0 %1 %2 %3] %4").arg(uAxis.x()).arg(uAxis.y()).arg(uAxis.z()).arg(translation.x()).arg(scale.x());
    QString sv = QString("[%0 %1 %2 %3] %4").arg(-vAxis.x()).arg(-vAxis.y()).arg(-vAxis.z()).arg(translation.y()).arg(scale.y());
    
    object.insert("uaxis", QJsonValue(su));
    object.insert("vaxis", QJsonValue(sv));
    
    return object;
}

QJsonObject DocumentSerialiser::convertBrush(const MapBrush *brush, int globalBrushId, int &globalFaceId)
{
    QJsonObject object;
    object.insert("editor", defaultEditorDataForSolid());
    object.insert("id", QJsonValue(QString("%0").arg(globalBrushId)));
    
    QJsonArray sideArray;
    for ( int i = 0; i < brush->faces().constData().count(); i++ )
    {
        sideArray.append(QJsonValue(convertFace(brush->faces(i), globalFaceId)));
        globalFaceId++;
    }
    
    object.insert("side", QJsonValue(sideArray));
    
    return object;
}

QJsonObject DocumentSerialiser::defaultEditorDataForSolid()
{
//    editor
//    {
//        "color" "0 149 102"
//        "visgroupshown" "1"
//        "visgroupautoshown" "1"
//    }
    
    QJsonObject object;
    
    object.insert("color", QJsonValue("255 255 255"));
    object.insert("visgroupshown", QJsonValue("1"));
    object.insert("visgroupautoshown", QJsonValue("1"));
    
    return object;
}

SERIALISATION_END_NAMESPACE
