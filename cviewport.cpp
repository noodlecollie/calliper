#include "cviewport.h"

CViewport::CViewport(QWidget * parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f), QOpenGLFunctions()
{
    
}

void CViewport::initializeGL()
{
    initializeOpenGLFunctions();
    
    glClearColor(0.0f, 0.58f, 1.0f, 1.0f);
}

void CViewport::resizeGL(int w, int h)
{
    
}


void CViewport::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
}
