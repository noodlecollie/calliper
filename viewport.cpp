#include "viewport.h"
#include "temporaryrender.h"
#include <QKeyEvent>
#include <QFocusEvent>

Viewport::Viewport(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
    m_Timer.connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));
    m_Timer.setInterval(0);

    m_colBackground = QColor::fromRgb(0xFF66CCFF);
    m_bBackgroundColorChanged = true;
}

Viewport::~Viewport()
{

}

void Viewport::updateBackgroundColor()
{
    if ( m_bBackgroundColorChanged )
    {
        glClearColor(m_colBackground.redF(), m_colBackground.greenF(), m_colBackground.blueF(), m_colBackground.alphaF());
        m_bBackgroundColorChanged = false;
    }
}

void Viewport::initializeGL()
{
    initializeOpenGLFunctions();

    updateBackgroundColor();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glGenVertexArrays(1, &m_iVertexArray);
    glBindVertexArray(m_iVertexArray);
}

void Viewport::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void Viewport::paintGL()
{
    updateBackgroundColor();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Viewport::keyPressEvent(QKeyEvent *e)
{
    Q_UNUSED(e);
}

void Viewport::keyReleaseEvent(QKeyEvent *e)
{
    Q_UNUSED(e);
}

void Viewport::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void Viewport::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void Viewport::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void Viewport::focusInEvent(QFocusEvent *e)
{
    Q_UNUSED(e);
    m_Timer.start();
}

void Viewport::focusOutEvent(QFocusEvent *e)
{
    Q_UNUSED(e);
    m_Timer.stop();
}

QColor Viewport::backgroundColor() const
{
    return m_colBackground;
}

void Viewport::setBackgroundColor(const QColor &col)
{
    m_colBackground = col;
    m_colBackground.setAlpha(255);
    m_bBackgroundColorChanged = true;
}
