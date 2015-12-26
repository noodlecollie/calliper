#include "openglrenderer.h"
#include <QOpenGLFunctions_4_1_Core>
#include "resourcemanager.h"

static OpenGLRenderer* g_pRenderer = NULL;
OpenGLRenderer* renderer()
{
    return g_pRenderer;
}

OpenGLRenderer::OpenGLRenderer()
{
    g_pRenderer = this;
}

OpenGLRenderer::~OpenGLRenderer()
{
    resourceManager()->makeCurrent();
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    // Nothing yet

    resourceManager()->doneCurrent();
}

void OpenGLRenderer::initialise()
{
    Q_ASSERT(!g_pRenderer);
    new OpenGLRenderer();
}

void OpenGLRenderer::shutdown()
{
    Q_ASSERT(g_pRenderer);
    delete g_pRenderer;
    g_pRenderer = NULL;
}

QColor OpenGLRenderer::globalColor() const
{
    return m_colGlobalColour;
}

void OpenGLRenderer::setGlobalColor(const QColor &col)
{
    m_colGlobalColour = col;
}

void OpenGLRenderer::setUpOpenGLResources()
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    setShaderIndex(0);
}

int OpenGLRenderer::shaderIndex() const
{
    return m_iShader;
}

void OpenGLRenderer::setShaderIndex(int index)
{
    m_iShader = index;
}
