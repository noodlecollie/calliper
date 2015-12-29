#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QColor>
#include "cameracontroller.h"
#include <QTime>

class Camera;
class GeometryData;
class Scene;

class Viewport : public QOpenGLWidget, public QOpenGLFunctions_4_1_Core
{
public:
    explicit Viewport(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~Viewport();

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &col);
    static QColor defaultBackgroundColor();

    Camera* camera() const;
    void setCamera(Camera* camera);

    Scene* scene() const;
    void setScene(Scene* scene);

    bool drawFocusHighlight() const;
    void setDrawFocusHighlight(bool enabled);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
    virtual void wheelEvent(QWheelEvent *e);

private:
    void updateBackgroundColor();
    void drawEmpty();
    QPoint viewCentre() const;
    void setCameraMouseControl(bool enabled);
    void drawHighlight();

    GLuint  m_iVertexArray;
    QTimer  m_Timer;

    Camera* m_pCamera;
    Scene*  m_pScene;
    QColor  m_colBackground;
    bool    m_bBackgroundColorChanged;
    CameraController    m_CameraController;
    QTime               m_TimeElapsed;

    GeometryData*       m_pEmptyText;
    GeometryData*       m_pHighlightOutline;

    bool    m_bMouseTracking;
    float   m_flMouseSensitivity;
    bool    m_bDrawFocusHighlight;
};

#endif // VIEWPORT_H
