#include "viewport.h"
#include "temporaryrender.h"
#include <QKeyEvent>

Viewport::Viewport(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{

}

void Viewport::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.58f, 1.0f, 1.0f);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glGenVertexArrays(1, &m_iVertexArray);
    glBindVertexArray(m_iVertexArray);

    m_Timer.connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));
    m_Timer.setInterval(0);
    m_Timer.start();

    temporarySetup(context(), this);
}

void Viewport::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void Viewport::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    temporaryRender(context(), this);
}

void Viewport::keyPressEvent(QKeyEvent *e)
{
    if ( temporaryKeyPress(e) )
        update();
}

void Viewport::keyReleaseEvent(QKeyEvent *e)
{
    if ( temporaryKeyRelease(e) )
        update();
}

void Viewport::mousePressEvent(QMouseEvent *e)
{
    if ( temporaryMousePress(e) )
        update();
}

void Viewport::mouseMoveEvent(QMouseEvent *e)
{
    if ( temporaryMouseMove(e) )
        update();
}

void Viewport::mouseReleaseEvent(QMouseEvent *e)
{
    if ( temporaryMouseRelease(e) )
        update();
}
