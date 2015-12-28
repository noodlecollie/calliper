#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QColor>

class Camera;
class GeometryData;

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

private:
    void updateBackgroundColor();
    void drawEmpty();

    GLuint  m_iVertexArray;
    QTimer  m_Timer;

    Camera* m_pCamera;
    QColor  m_colBackground;
    bool    m_bBackgroundColorChanged;
    GeometryData*   m_pEmptyText;
};

#endif // VIEWPORT_H
