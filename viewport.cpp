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
#include "basiclittextureshader.h"

Viewport::Viewport(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
    m_Timer.connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));
    m_Timer.setInterval(0);

    m_colBackground = Viewport::defaultBackgroundColor();
    m_bBackgroundColorChanged = true;
    m_pCamera = NULL;
    m_pScene = NULL;
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
                                              "No active document",
                                              QColor::fromRgb(0xffd9d9d9),
                                              QFont("Arial", 25),
                                              Qt::AlignCenter);
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

    if ( !m_pCamera || !m_pScene )
    {
        drawEmpty();
        return;
    }

    // TODO: Should these calls be somewhere else?
    int index = resourceManager()->shaderIndex(BasicLitTextureShader::staticName());
    Q_ASSERT(index >= 0);
    renderer()->setShaderIndex(index);

    renderer()->preRender();
    renderer()->renderScene(m_pScene, m_pCamera);
    renderer()->postRender();
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

QColor Viewport::defaultBackgroundColor()
{
    return QColor::fromRgb(0xff33001a);
}

void Viewport::drawEmpty()
{
    // TODO: Put this type of thing into the renderer and make it generic.

    // We can't use QPainter because there's a bug with Qt
    // where the OpenGL shaders won't compile on Mac due to
    // a missing #version specifier.
    Q_ASSERT(m_pEmptyText);

    m_pEmptyText->upload();
    m_pEmptyText->bindVertices(true);
    m_pEmptyText->bindIndices(true);

    ShaderProgram* program = resourceManager()->shader("UnlitTextureShader");
    program->apply();

    m_pEmptyText->applyDataFormat(program);
    program->setUniformMatrix4(ShaderProgram::ModelToWorldMatrix, QMatrix4x4());
    program->setUniformMatrix4(ShaderProgram::WorldToCameraMatrix, QMatrix4x4());
    program->setUniformMatrix4(ShaderProgram::CoordinateTransformMatrix, QMatrix4x4());
    program->setUniformMatrix4(ShaderProgram::CameraProjectionMatrix, QMatrix4x4());

    m_pEmptyText->localTexture()->bind(0);
    m_pEmptyText->draw();

    program->release();
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
