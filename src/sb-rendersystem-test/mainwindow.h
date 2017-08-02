#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QTimer>
#include <QOpenGLFramebufferObject>

//#define FOLLOW_FBO_EXAMPLE

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    void setOpenGLOptionsForBoundFrameBuffer();
    void drawCube();
    void drawQuad();

#ifdef FOLLOW_FBO_EXAMPLE
    void generateTexture();
    void generateRenderBuffer();
#endif

#ifndef FOLLOW_FBO_EXAMPLE
    QOpenGLFramebufferObject* m_pFrameBuffer;
#else
    GLuint m_nFBOID;
    GLuint m_nRBID;
    GLuint m_nFBTextureID;
#endif

    QOpenGLShaderProgram* m_pCubeShaderProgram;
    QOpenGLShaderProgram* m_pQuadShaderProgram;

    QOpenGLBuffer* m_pCubeVertexBuffer;
    QOpenGLBuffer* m_pCubeIndexBuffer;

    QOpenGLBuffer* m_pQuadVertexBuffer;
    QOpenGLBuffer* m_pQuadIndexBuffer;

    QTimer m_RefreshTimer;
    bool m_bInitialised;
    quint32 m_nFrames;

    GLuint m_nVAOID;
    int m_nModelToWorldMatrixLocation;
};

#endif // MAINWINDOW_H
