#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class Window : public QOpenGLWindow
{
public:
    Window();
    virtual ~Window();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    void releaseAll();

    GLuint m_VAOID;
    QOpenGLShaderProgram* m_pShaderProgram;
    QOpenGLBuffer* m_pVertexBuffer;
    QOpenGLBuffer* m_pIndexBuffer;
};

#endif // WINDOW_H
