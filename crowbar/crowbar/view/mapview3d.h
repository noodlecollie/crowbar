#ifndef MAPVIEW3D_H
#define MAPVIEW3D_H

#include "mapview.h"

class QGLSceneNode;

VIEW_BEGIN_NAMESPACE

class UserControlledCamera;

class VIEWSHARED_EXPORT MapView3D : public MapView
{
    Q_OBJECT
    Q_PROPERTY(float horizontalSensitivity READ horizontalSensitivity WRITE setHorizontalSensitivity)
    Q_PROPERTY(float verticalSensitivity READ verticalSensitivity WRITE setVerticalSensitivity)
    Q_PROPERTY(bool cameraControl READ cameraControl)
public:
    explicit MapView3D(QWidget* parent = 0, GLAngleCamera* defaultCamera = NULL);
    virtual ~MapView3D();

    float horizontalSensitivity() const;
    void setHorizontalSensitivity(float sensitivity);

    float verticalSensitivity() const;
    void setVerticalSensitivity(float sensitivity);

    bool cameraControl() const;

    UserControlledCamera* userCamera();
    const UserControlledCamera* userCamera() const;

    UserControlledCamera* defaultUserCamera();
    const UserControlledCamera* defaultUserCamera() const;

    virtual IRender::RenderMode renderMode() const;

    bool inMouseLook() const;
    void cancelMouseLook();

public slots:

private slots:
    void disableCameraControl();
    void enableCameraControl();

signals:
    void invertedScrollChanged(bool);

protected:
    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void focusInEvent(QFocusEvent *);
    virtual void focusOutEvent(QFocusEvent *);
    virtual void wheelEvent(QWheelEvent *);

    virtual void prePaintGL(QGLPainter *);
    virtual void paintGL(QGLPainter *);
    virtual void postPaintGL(QGLPainter *);
    virtual void virtualInitGL();
    
    virtual void vDragBegin(const QPoint &p);
    virtual void vDragMove(const QPoint &p);
    virtual void vDragEnd();
    
    virtual void vRightDragBegin(const QPoint &p);
    virtual void vRightDragMove(const QPoint &p);
    virtual void vRightDragEnd();

private:
    void initialise();
    void processMouseMovement(const QPoint &pos);
    void toggleMouseGrab();
    void moveCursorToWidgetCentre();

    float           m_flHSensitivity;
    float           m_flVSensitivity;
    bool            m_bGrabMouse;
    bool            m_bDisableCameraControl;
    QGLSceneNode*   m_pOrigin;
};

VIEW_END_NAMESPACE

#endif // MAPVIEW3D_H
