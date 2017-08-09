#ifndef CUSTOMOPENGLWIDGET_H
#define CUSTOMOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>
#include <QOffscreenSurface>

class CustomOpenGLWidget : public QWidget
{
public:
    CustomOpenGLWidget(QOpenGLContext* context, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~CustomOpenGLWidget();

    bool isInitialised() const;
    QOpenGLContext* context() const;
    void makeCurrentWithSurface();
    void doneCurrent();
    void bindFBO();
    void releaseFBO();
    void flagFlushPending();

private:
    void initialiseTriangleResources();
    void destroyTriangleResources();
    void drawTriangle();

    void recreateFrameBuffer();

    QOpenGLShaderProgram* m_pShaderProgram;
    QOpenGLBuffer* m_pVertexBuffer;
    QOpenGLBuffer* m_pIndexBuffer;
    GLuint m_nVAOID;
    bool m_bInitialised;

    QOpenGLContext* m_pContext;
    bool m_bFlushPending;
    QOpenGLFramebufferObject* m_pFrameBuffer;
    QOffscreenSurface* m_pOffscreenSurface;
};

#endif // CUSTOMOPENGLWIDGET_H
