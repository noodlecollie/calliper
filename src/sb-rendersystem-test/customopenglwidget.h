#ifndef CUSTOMOPENGLWIDGET_H
#define CUSTOMOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class CustomOpenGLWidget : public QOpenGLWidget
{
public:
    CustomOpenGLWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~CustomOpenGLWidget();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    void initialiseTriangleResources();
    void destroyTriangleResources();
    void drawTriangle();

    QOpenGLShaderProgram* m_pShaderProgram;
    QOpenGLBuffer* m_pVertexBuffer;
    QOpenGLBuffer* m_pIndexBuffer;
    GLuint m_nVAOID;
    bool m_bInitialised;
};

#endif // CUSTOMOPENGLWIDGET_H
