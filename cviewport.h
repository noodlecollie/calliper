#ifndef CVIEWPORT_H
#define CVIEWPORT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>

class CViewport : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit CViewport(QWidget * parent = 0, Qt::WindowFlags f = 0);
    
signals:
    
public slots:
    
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    GLuint vertexbuffer;
    GLuint ProgramID;
    QMatrix4x4 MVP;
    GLuint MatrixID;
};

#endif // CVIEWPORT_H
