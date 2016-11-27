#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QColor>
#include "cameracontroller.h"
#include <QTime>
#include "sceneobject.h"

class SceneCamera;
class GeometryData;
class ViewportUserOptions;
class QPushButton;
class MapDocument;

class Viewport : public QOpenGLWidget, public QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    explicit Viewport(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~Viewport();

    // On high DPI displays, size() doesn't return the correct size for the OpenGL canvas.
    virtual QSize sizeInPixels() const;
    QPoint viewCentre() const;

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &col);
    static QColor defaultBackgroundColor();

    SceneCamera* camera() const;
    void setCamera(SceneCamera* camera);

    MapDocument* document() const;
    void setDocument(MapDocument* doc);

    bool drawFocusHighlight() const;
    bool drawFPS() const;

    SceneObject* pickObjectFromDepthBuffer(int sceneFlags, const QPoint &pos, int selectionMask = SceneObject::AllObjectsMask, QRgb* pickColor = nullptr);
    bool saveCurrentFrame(const QString &filename);

    void repaintHack();

signals:
    void preFrame(int);

public slots:
    void setDrawFocusHighlight(bool enabled);
    void setDrawFPS(bool enabled);

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
    enum RenderTask
    {
        DepthBufferSelect = 0x1,
        SaveCurrentFrame = 0x2
    };

    void updateBackgroundColor();
    void drawEmpty();
    void drawNoActiveCamera();
    void drawHighlight();
    void drawFPSText(int msec);
    void debugSaveCurrentFrame();
    void drawScene();
    void processRenderTasks();
    void selectFromDepthBuffer(const QPoint &pos);
    void saveCurrentFrame();

    GLuint  m_iVertexArray;
    QTimer  m_Timer;

    SceneCamera* m_pCamera;
    MapDocument* m_pDocument;
    QColor  m_colBackground;
    bool    m_bBackgroundColorChanged;
    QTime   m_TimeElapsed;

    ViewportUserOptions*    m_pUserOptions;
    QPushButton*            m_pToggleOptions;

    GeometryData*       m_pEmptyText;
    GeometryData*       m_pNoCameraText;
    GeometryData*       m_pHighlightOutline;

    bool    m_bDrawFocusHighlight;
    bool    m_bDrawFPS;
    int     m_iRenderTasks;
    QString m_szSaveFrameFilename;
    bool    m_bSaveFrameResult;

    QPoint          m_DepthSelectPos;
    SceneObject*    m_pPickedObject;
    QRgb            m_PickColour;
    int             m_iScenePickFlags;
    int             m_iObjectSelectionMask;
};

#endif // VIEWPORT_H
