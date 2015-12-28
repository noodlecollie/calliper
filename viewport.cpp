#include "viewport.h"
#include <QKeyEvent>
#include <QFocusEvent>
#include "openglrenderer.h"
#include "geometrydata.h"
#include <QOpenGLTexture>
#include "resourcemanager.h"
#include "shaderprogram.h"
#include <QMatrix4x4>
#include "scene.h"
#include "shaders.h"
#include "camera.h"

Viewport::Viewport(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
    m_Timer.connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));
    m_Timer.setInterval(0);

    m_colBackground = Viewport::defaultBackgroundColor();
    m_bBackgroundColorChanged = true;
    m_pCamera = NULL;
    m_pScene = NULL;

    m_CameraController.setTopSpeed(10.0f);
}

Viewport::~Viewport()
{
    makeCurrent();
    delete m_pEmptyText;
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &m_iVertexArray);
    glBindVertexArray(m_iVertexArray);

    m_pEmptyText = renderer()->createTextQuad(QSize(256,256),
                                              "No active\ndocument",
                                              QColor::fromRgb(0xffd9d9d9),
                                              QFont("Arial", 25),
                                              Qt::AlignCenter);

    m_TimeElapsed.start();
}

void Viewport::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void Viewport::paintGL()
{
    int msec = m_TimeElapsed.restart();
    m_CameraController.update(msec);

    updateBackgroundColor();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if ( !m_pCamera || !m_pScene )
    {
        drawEmpty();
        return;
    }

    m_pCamera->translate(m_CameraController.velocity());

    // TODO: Should these calls be somewhere else?
    int index = resourceManager()->shaderIndex(BasicLitTextureShader::staticName());
    Q_ASSERT(index >= 0);
    renderer()->setShaderIndex(index);

    renderer()->begin();
    renderer()->renderScene(m_pScene, m_pCamera);
    renderer()->end();
}

void Viewport::keyPressEvent(QKeyEvent *e)
{
    if ( e->isAutoRepeat() )
    {
        QOpenGLWidget::keyPressEvent(e);
        return;
    }

    switch (e->key())
    {
        case Qt::Key_W:
        {
            m_CameraController.forward(true);
            break;
        }
        case Qt::Key_A:
        {
            m_CameraController.left(true);
            break;
        }
        case Qt::Key_D:
        {
            m_CameraController.right(true);
            break;
        }
        case Qt::Key_S:
        {
            m_CameraController.backward(true);
            break;
        }

        default:
        {
            QOpenGLWidget::keyPressEvent(e);
            break;
        }
    }
}

void Viewport::keyReleaseEvent(QKeyEvent *e)
{
    if ( e->isAutoRepeat() )
    {
        QOpenGLWidget::keyPressEvent(e);
        return;
    }

    switch (e->key())
    {
        case Qt::Key_W:
        {
            m_CameraController.forward(false);
            break;
        }
        case Qt::Key_A:
        {
            m_CameraController.left(false);
            break;
        }
        case Qt::Key_D:
        {
            m_CameraController.right(false);
            break;
        }
        case Qt::Key_S:
        {
            m_CameraController.backward(false);
            break;
        }

        default:
        {
            QOpenGLWidget::keyPressEvent(e);
            break;
        }
    }
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
    m_CameraController.reset();
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

QColor Viewport::defaultBackgroundColor()
{
    return QColor::fromRgb(0xff33001a);
}

void Viewport::drawEmpty()
{
    int index = resourceManager()->shaderIndex(UnlitTextureShader::staticName());
    Q_ASSERT(index >= 0);
    renderer()->setShaderIndex(index);

    QSize s = size();
    int shortestWin = qMin(s.width(), s.height());
    int shortestTex = qMin(m_pEmptyText->localTexture()->width(), m_pEmptyText->localTexture()->height());
    int dimension = qMin(shortestTex, shortestWin);
    if ( dimension < 1 ) return;

    int x = s.width()/2;
    int y = s.height()/2;

    renderer()->begin();
    renderer()->drawQuad(m_pEmptyText, s, QRect(x, y, dimension, dimension));
    renderer()->end();
}

Camera* Viewport::camera() const
{
    return m_pCamera;
}

void Viewport::setCamera(Camera *camera)
{
    m_pCamera = camera;
}

Scene* Viewport::scene() const
{
    return m_pScene;
}

void Viewport::setScene(Scene *scene)
{
    m_pScene = scene;
}
