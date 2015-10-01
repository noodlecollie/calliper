#ifndef CVIEWPORT_H
#define CVIEWPORT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>
#include <QMatrix4x4>

class CViewport : public QOpenGLWidget, protected QOpenGLFunctions_3_2_Core
{
    Q_OBJECT
public:
    explicit CViewport(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~CViewport();
    
signals:
    
public slots:
    
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    virtual void keyPressEvent(QKeyEvent *e);

private:
    GLuint vertexbuffer;
    GLuint colorbuffer;
    GLuint textureID;
    GLuint uvbuffer;
    GLuint textureBuffer;
    GLuint ProgramID;
    QMatrix4x4 MVP;
    GLuint MatrixID;
    GLuint VertexShaderID;
    GLuint FragmentShaderID;
    GLuint VertexArrayID;

    float Top;
    float Bottom;
    float Left;
    float Right;
    float Near;
    float Far;

    QVector3D camPos;
    bool usePerspective;
};

#endif // CVIEWPORT_H
