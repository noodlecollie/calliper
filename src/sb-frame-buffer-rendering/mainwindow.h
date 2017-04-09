#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include "renderer/opengl/openglvertexarrayobject.h"

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWindow *parent = 0);
    ~MainWindow();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    Renderer::OpenGLVertexArrayObject m_VAO;
};

#endif // MAINWINDOW_H
