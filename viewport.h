#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>

class Viewport : public QOpenGLWidget, public QOpenGLFunctions_4_1_Core
{
public:
    Viewport(QWidget* parent = 0, Qt::WindowFlags f = 0);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);

private:
    GLuint  m_iVertexArray;
};

#endif // VIEWPORT_H
