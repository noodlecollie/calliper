#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>

class Viewport : public QOpenGLWidget, public QOpenGLFunctions_4_1_Core
{
public:
    explicit Viewport(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~Viewport();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    GLuint  m_iVertexArray;
    QTimer  m_Timer;
};

#endif // VIEWPORT_H
