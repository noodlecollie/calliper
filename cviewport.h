#ifndef CVIEWPORT_H
#define CVIEWPORT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>
#include <QMatrix4x4>
#include "cbasiccamera.h"

// Testing - upload a texture through the global context and see
// if it's accessible through the widget.
extern GLuint gTextureBuffer;
class CVertexBundle;
class CDebugCube;

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
    virtual void keyReleaseEvent(QKeyEvent *e);

private:
    GLuint vertexbuffer;
    GLuint colorbuffer;
    GLuint textureID;
    GLuint uvbuffer;
    GLuint texturebuffer;
    GLuint ProgramID;
    QMatrix4x4 MVP;
    GLuint MatrixID;
    GLuint VertexShaderID;
    GLuint FragmentShaderID;
    GLuint VertexArrayID;
    GLuint IndexArrayID;

    float Top;
    float Bottom;
    float Left;
    float Right;
    float Near;
    float Far;

    QVector3D camPos;
    bool useGlobalTexture;
    bool ctrlPressed;
    bool useCoordTransform;

    CBasicCamera camera;
    CDebugCube* debugCube;
};

#endif // CVIEWPORT_H
