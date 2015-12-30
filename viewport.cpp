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
#include <QCursor>
#include "geometryfactory.h"
#include <QWheelEvent>
#include "viewportuseroptions.h"
#include <QPushButton>
#include "simplenumericfont.h"

Viewport::Viewport(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
    m_colBackground = Viewport::defaultBackgroundColor();
    m_bBackgroundColorChanged = true;
    m_pCamera = NULL;
    m_pScene = NULL;
    m_bMouseTracking = false;
    m_flMouseSensitivity = 1.5f;
    m_bDrawFocusHighlight = false;
    m_bDrawFPS = false;

    m_pToggleOptions = new QPushButton(QIcon(QPixmap::fromImage(QImage(":/icons/viewport_options.png"))), QString(), this);
    m_pToggleOptions->resize(18,14);

    m_pUserOptions = new ViewportUserOptions(this);
    m_pUserOptions->move(2,16);
    m_pUserOptions->setVisible(false);

    connect(m_pToggleOptions, &QPushButton::clicked, m_pUserOptions, &ViewportUserOptions::toggleVisibility);
    connect(m_pUserOptions, &ViewportUserOptions::focusHighlightStatusChanged, this, &Viewport::setDrawFocusHighlight);
    connect(m_pUserOptions, &ViewportUserOptions::fpsStatusChanged, this, &Viewport::setDrawFPS);

    m_Timer.connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));
    m_Timer.setInterval(0);

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

    m_pHighlightOutline = GeometryFactory::lineRect(1.0f, QColor::fromRgb(0xffff0000));

    m_TimeElapsed.start();
}

void Viewport::resizeGL(int w, int h)
{
    if ( m_pCamera )
    {
        CameraLens lens = m_pCamera->lens();
        lens.setAspectRatio((float)w/(float)h);
        m_pCamera->setLens(lens);
        update();
        return;
    }

    QOpenGLWidget::resizeGL(w,h);
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

    if ( hasFocus() && m_bDrawFocusHighlight )
        drawHighlight();

    if ( m_bDrawFPS )
        drawFPSText(msec);

    int index = resourceManager()->shaderIndex(BasicLitTextureShader::staticName());
    Q_ASSERT(index >= 0);
    renderer()->setShaderIndex(index);

    renderer()->begin();
    //renderer()->renderScene(m_pScene, m_pCamera);
    renderer()->renderScene2(m_pScene, m_pCamera);
    renderer()->end();
}

void Viewport::drawHighlight()
{
    int index;
    index = resourceManager()->shaderIndex(PerVertexColorShader::staticName());
    Q_ASSERT(index >= 0);
    renderer()->setShaderIndex(index);
    renderer()->begin();
    renderer()->drawQuad(m_pHighlightOutline, size(), QRect(0, 0, width()-1, height()-1), Qt::AlignLeft | Qt::AlignTop);
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

        case Qt::Key_Z:
        {
            if ( !m_pCamera )
                break;

            setCameraMouseControl(!m_bMouseTracking);
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
    if ( !m_bMouseTracking || !m_pCamera )
    {
        QOpenGLWidget::mouseMoveEvent(e);
        return;
    }

    QPoint p = e->pos();
    QPoint delta = p - viewCentre();

    EulerAngle angles = m_pCamera->angles();
    angles.setPitch(angles.pitch() + (m_flMouseSensitivity * delta.y()));
    angles.setYaw(angles.yaw() - (m_flMouseSensitivity * delta.x()));
    m_pCamera->setAngles(angles);

    QCursor::setPos(mapToGlobal(viewCentre()));

    update();
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
    setCameraMouseControl(false);
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
    CameraLens lens = m_pCamera->lens();
    lens.setAspectRatio((float)size().width()/(float)size().height());
    m_pCamera->setLens(lens);
}

Scene* Viewport::scene() const
{
    return m_pScene;
}

void Viewport::setScene(Scene *scene)
{
    m_pScene = scene;
}

QPoint Viewport::viewCentre() const
{
    return QPoint(size().width()/2, size().height()/2);
}

void Viewport::setCameraMouseControl(bool enabled)
{
    if ( enabled )
    {
        QCursor::setPos(mapToGlobal(viewCentre()));
        setMouseTracking(true);
        setCursor(Qt::BlankCursor);
    }
    else
    {
        setMouseTracking(false);
        setCursor(Qt::ArrowCursor);
    }

    m_bMouseTracking = enabled;
}

bool Viewport::drawFocusHighlight() const
{
    return m_bDrawFocusHighlight;
}

void Viewport::setDrawFocusHighlight(bool enabled)
{
    if ( m_bDrawFocusHighlight == enabled ) return;

    m_bDrawFocusHighlight = enabled;
}

bool Viewport::drawFPS() const
{
    return m_bDrawFPS;
}

void Viewport::setDrawFPS(bool enabled)
{
    if ( m_bDrawFPS == enabled ) return;

    m_bDrawFPS = enabled;
}

void Viewport::wheelEvent(QWheelEvent *e)
{
    if ( !m_pCamera )
    {
        QOpenGLWidget::wheelEvent(e);
        return;
    }

    int delta = e->delta();
    m_pCamera->translate(QVector3D(delta,0,0));
    update();
}

void Viewport::drawFPSText(int msec)
{
    float framesPerSecond = 1.0f/((float)msec/1000.0f);

    int index;
    index = resourceManager()->shaderIndex(UnlitTextureShader::staticName());
    Q_ASSERT(index >= 0);
    renderer()->setShaderIndex(index);
    renderer()->begin();
    resourceManager()->numericFont()->draw(QString("%0").arg(framesPerSecond).toLatin1(), size(), QSize(16,16), QPoint(20,0));
    renderer()->end();
}
