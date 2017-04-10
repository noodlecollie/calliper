#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

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
    QOpenGLBuffer m_VertexBuffer;
    QOpenGLBuffer m_IndexBuffer;
    QOpenGLShaderProgram m_ShaderProgram;
    int m_iVertexLocation;
    bool m_bInitialised;
};

#endif // MAINWINDOW_H
