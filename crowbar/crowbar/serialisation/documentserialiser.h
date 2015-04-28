#ifndef DOCUMENTSERIALISER_H
#define DOCUMENTSERIALISER_H

#include <QObject>
#include <QJsonDocument>
#include "mapdocument.h"
#include "mapface.h"
#include "serialisation_global.h"

/**
 * JSON specification:
 * {
 *      "metadata":
 *      {
 *          ...
 *      },
 *      
 *      "objects":
 *      {
 *          "brushes":
 *          [
 *              {
 *                  "vertices":
 *                  [
 *                      {
 *                          "position":
 *                          [
 *                              <x>,
 *                              <y>,
 *                              <z>
 *                          ],
 * 
 *                          "color:"
 *                          [
 *                              <r>,
 *                              <g>,
 *                              <b>
 *                          ]
 *                      },
 *                      ...
 *                  ],
 * 
 *                  "edges":
 *                  [
 *                      {
 *                          "vertex0": <index>,
 *                          "vertex1": <index>
 *                      },
 *                      ...
 *                  ],
 * 
 *                  "faces":
 *                  [
 *                      {
 *                          "vertices":
 *                          [
 *                              <index>,
 *                              ...
 *                          ],
 *                          
 *                          "textureIndex": <value>
 *                      }
 *                  ]
 *              },
 *              ...
 *          ]
 *      }
 * }
 */

SERIALISATION_BEGIN_NAMESPACE

class SERIALISATIONSHARED_EXPORT DocumentSerialiser : public QObject
{
    Q_OBJECT
public:
    explicit DocumentSerialiser(QObject *parent = 0);
    
    static void toVMF(const MODEL_NAMESPACE::MapDocument* document, QByteArray &vmf);
    
signals:
    
public slots:
    
private:
    static QJsonObject defaultVersionInfo();
    static QJsonObject defaultViewSettings();
    static QJsonObject defaultCameras();
    static QJsonObject defaultCordons();
    static QJsonObject defaultEditorDataForSolid();
    static QJsonObject emptyWorld();
    static void populateEmptyWorld(const MODEL_NAMESPACE::MapDocument* document, QJsonObject &world);
    static QJsonObject convertFace(const MODEL_NAMESPACE::MapFace* face, int id);
    static QJsonObject convertBrush(const MODEL_NAMESPACE::MapBrush* brush, int globalBrushId, int &globalFaceId);
};

SERIALISATION_END_NAMESPACE

#endif // DOCUMENTSERIALISER_H
