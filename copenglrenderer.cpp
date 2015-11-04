#include "copenglrenderer.h"
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QGuiApplication>
#include <QOpenGLShaderProgram>

COpenGLRenderer::COpenGLRenderer(QObject *parent) : CBaseRenderer(parent)
{
    m_pBackgroundContext = NULL;
}

bool COpenGLRenderer::createBackgroundContext()
{
    Q_ASSERT(!m_pBackgroundContext);

    m_pBackgroundContext = new QOpenGLContext(this);
    m_pBackgroundContext->setFormat(QSurfaceFormat::defaultFormat());
    m_pBackgroundContext->setScreen(QGuiApplication::screens().at(0));
    m_pBackgroundContext->setShareContext(QOpenGLContext::globalShareContext());

    return m_pBackgroundContext->create();
}

QOpenGLContext* COpenGLRenderer::backgroundContext() const
{
    return m_pBackgroundContext;
}

void COpenGLRenderer::initialise()
{
    createBackgroundContext();
    compileShaders();
}

bool COpenGLRenderer::isValid() const
{
    return m_pBackgroundContext && m_pBackgroundContext->isValid();
}

bool COpenGLRenderer::compileShaders()
{
    // Solid colour
    QOpenGLShaderProgram* p = new QOpenGLShaderProgram();
    m_ShaderList.append(p);
    p->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/TODO");
}
