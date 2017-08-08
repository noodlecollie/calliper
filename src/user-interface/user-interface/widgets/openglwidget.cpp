#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
    // setRenderToTexture() (?)
}

OpenGLWidget::~OpenGLWidget()
{
    // reset()
}

void OpenGLWidget::makeCurrent()
{

}

void OpenGLWidget::doneCurrent()
{

}
