#ifndef MAPDOCUMENTVIEW_H
#define MAPDOCUMENTVIEW_H

#include <QObject>
#include "view_global.h"
#include "model_global.h"
#include <QPointer>
#include <QHash>
#include "rendertableentries/rendertableentry.h"
#include "irender.h"
#include <QRect>
#include <QGLPainter>
#include <QSet>

class QGLSceneNode;
class QGLLightModel;

MODEL_BEGIN_NAMESPACE
class MapDocument;
class MapComponent;
class MapFace;
MODEL_END_NAMESPACE

VIEW_BEGIN_NAMESPACE

class MapView;

class VIEWSHARED_EXPORT MapDocumentView : public QObject, public IRender
{
    Q_OBJECT
public:
    typedef QHash<MODEL_NAMESPACE::MapComponent*, RenderTableEntry*> RenderTable;

    explicit MapDocumentView(MODEL_NAMESPACE::MapDocument *document, QObject* parent = 0);
    ~MapDocumentView();

    MODEL_NAMESPACE::MapDocument* document() const;

    QGLSceneNode* sceneNode(MODEL_NAMESPACE::MapComponent* component, IRender::RenderMode mode) const;
    bool contains(MODEL_NAMESPACE::MapComponent* component) const;
    int count() const;

    virtual void draw(QGLPainter *painter, RenderMode mode, const QSize &viewSize);

    // Returns all objects that "own" pixels within the given area.
    virtual QList<MODEL_NAMESPACE::MapComponent*>
    objectsWithin(const QRect &selection, QGLPainter *painter, RenderMode mode, const QSize &size);
    virtual MODEL_NAMESPACE::MapFace* faceAt(const QPoint &point, QGLPainter *painter, RenderMode mode, const QSize &size);

    virtual bool supportsObjectPicking() const { return true; }
    
    virtual QList<const QGLLightParameters*> lights() const;
    virtual QGLLightModel* lightModel();
    virtual const QGLLightModel* lightModel() const;
    virtual QBox3D selectionBounds() const;

signals:

public slots:
    void documentComponentAdded(MODEL_NAMESPACE::MapComponent* component);
    void documentComponentRemoved(MODEL_NAMESPACE::MapComponent* component);

private slots:
    void handleComponentDeleted(MODEL_NAMESPACE::MapComponent* component);

private:
    template<typename T>
    class SelectionManager
    {
    public:
        SelectionManager(const QRect &area, QGLPainter* painter)
        {
            m_Selection = area;

            if ( !area.isValid() )
            {
                m_bIsValid = false;
                m_pPreBuffer = NULL;
                m_pPostBuffer = NULL;
                m_pComponentArray = NULL;
                m_iSelectionArea = 0;
                return;
            }

            m_bIsValid = true;

            m_iSelectionArea = area.width() * area.height();
            painter->glClear(GL_DEPTH_BUFFER_BIT);

            m_pPreBuffer = new float[m_iSelectionArea];
            painter->glReadPixels(area.x(), area.y(), area.width(), area.height(),
                                  GL_DEPTH_COMPONENT, GL_FLOAT, m_pPreBuffer);

            m_pPostBuffer = new float[m_iSelectionArea];
            memcpy(m_pPostBuffer, m_pPreBuffer, m_iSelectionArea);

            m_pComponentArray = new T*[m_iSelectionArea];
            memset(m_pComponentArray, 0, m_iSelectionArea * sizeof(T*));
        }

        ~SelectionManager()
        {
            if ( m_pPreBuffer )
            {
                delete[] m_pPreBuffer;
                m_pPreBuffer = NULL;
            }

            if ( m_pPostBuffer )
            {
                delete[] m_pPostBuffer;
                m_pPostBuffer = NULL;
            }

            if ( m_pComponentArray )
            {
                delete[] m_pComponentArray;
                m_pComponentArray = NULL;
            }
        }

        inline bool isValid() const { return m_bIsValid; }

        // Component is the component that has just been drawn.
        void compareValues(QGLPainter* painter, T* component)
        {
            if ( !isValid() ) return;

            painter->glReadPixels(m_Selection.x(), m_Selection.y(), m_Selection.width(), m_Selection.height(),
                                  GL_DEPTH_COMPONENT, GL_FLOAT, m_pPostBuffer);

            // Check whether any values changed.
            for ( int i = 0; i < m_iSelectionArea; i++ )
            {
                // Let us know if the depth value increased (that'd be weird).
                if ( m_pPostBuffer[i] > m_pPreBuffer[i] )
                {
                    qWarning() << "WARNING: depth value" << m_pPostBuffer[i] << "increased from previous value"
                               << m_pPreBuffer[i] << "when drawing component" << component << "- check this out.";
                }

                // If the depth value changed, record the object.
                if ( m_pPreBuffer[i] != m_pPostBuffer[i] )
                {
                    m_pComponentArray[i] = component;
                }
            }

            // Swap the buffers so that the new values become old.
            float* temp = m_pPreBuffer;
            m_pPreBuffer = m_pPostBuffer;
            m_pPostBuffer = temp;
        }

        QList<T*> objects() const
        {
            if ( !isValid() ) return QList<T*>();

            QSet<T*> set;

            for ( int i = 0; i < m_iSelectionArea; i++ )
            {
                if ( m_pComponentArray[i] ) set.insert(m_pComponentArray[i]);
            }

            return set.toList();
        }

    private:
        QRect   m_Selection;
        bool    m_bIsValid;
        float*  m_pPreBuffer;
        float*  m_pPostBuffer;
        int     m_iSelectionArea;
        T**     m_pComponentArray;
    };

    typedef SelectionManager<MODEL_NAMESPACE::MapComponent> ComponentSelectionManager;
    typedef SelectionManager<MODEL_NAMESPACE::MapFace> FaceSelectionManager;

    void populateFromDocument();
    void clearAll();
    void populateRecursive(MODEL_NAMESPACE::MapComponent* c);
    void drawRecursive(MODEL_NAMESPACE::MapComponent* component, QGLPainter* painter, RenderMode mode,
                       const QSize &viewSize, ComponentSelectionManager* selection = NULL);
    void updateRenderEntryFor(MODEL_NAMESPACE::MapComponent* component);
    void removeRenderEntryFor(MODEL_NAMESPACE::MapComponent* component);
    void drawSelectedBounds(QGLPainter* painter);

    QPointer<MODEL_NAMESPACE::MapDocument> m_pDocument;
    RenderTable m_RenderTable;

    QRect m_TempSelection;
    IRender::RenderMode m_iTempMode;
    QList<MODEL_NAMESPACE::MapComponent*> m_TempComponentList;
    QGLLightModel* m_pLightModel;
    QBox3D m_SelectedBounds;
    QGLSceneNode* m_pSelectedBoundsNode;
};

VIEW_END_NAMESPACE

#endif // MAPDOCUMENTVIEW_H
