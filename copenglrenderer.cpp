#include "copenglrenderer.h"
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QGuiApplication>
#include <QOpenGLShaderProgram>
#include <QtDebug>
#include <QMetaEnum>

// Size of each attribute in bytes.
static const int ATTRIBUTE_SIZE[] = {
    3 * sizeof(float),      // Position
    2 * sizeof(float),      // UV
};

// Size of each interleaving format in bytes.
static const int FORMAT_SIZE[] = {
    ATTRIBUTE_SIZE[COpenGLRenderer::Position],
    ATTRIBUTE_SIZE[COpenGLRenderer::Position] + ATTRIBUTE_SIZE[COpenGLRenderer::UV],
};

int COpenGLRenderer::attributeOffset(InterleavingFormat format, Attribute att)
{
    switch (format)
    {
        case FormatPosition:
        {
            switch (att)
            {
            case Position:
                return 0;
            default:
                return 0;
            }
        }

        case FormatPositionUV:
        {
            switch(att)
            {
            case Position:
                return 0;
            case UV:
                return 3 * sizeof(float);
            default:
                return 0;
            }
        }

    default:
        return 0;
    }
}

int COpenGLRenderer::attributeSize(Attribute att)
{
    return ATTRIBUTE_SIZE[att];
}

int COpenGLRenderer::interleavingFormatSize(InterleavingFormat format)
{
    return FORMAT_SIZE[format];
}

bool attemptCompile(QOpenGLShaderProgram* program, const QString &shaderName, QOpenGLShader::ShaderType type, const QString &path)
{
    if ( !program->addShaderFromSourceFile(type, path) )
    {
        qCritical().nospace() << "Compilation failed for " << shaderName << " with shader file " << path << ": " << program->log();
        return false;
    }

    return true;
}

bool attemptLink(QOpenGLShaderProgram* program, const QString &shaderName)
{
    if ( !program->link() )
    {
        qCritical().nospace() << "Linking failed for shader " << shaderName << ": " << program->log();
        return false;
    }

    return true;
}

COpenGLRenderer::COpenGLRenderer(QObject *parent) : QObject(parent)
{
    m_pBackgroundContext = NULL;
    m_bInitialiseAttempted = false;
}

COpenGLRenderer::~COpenGLRenderer()
{
    // Order of deletion:
    // - Scene objects (buffers etc)
    // - Shaders
    // - Background context

    qDeleteAll(m_ShaderList);
    delete m_pBackgroundContext;
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
    // If anything fails, record that we tried and exit out.
    switch (0)
    {
        default:
        if ( !createBackgroundContext() ) break;
        if ( !compileShaders() ) break;
    }

    m_bInitialiseAttempted = true;
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

    bool success = true;
    success = attemptCompile(p, "Solid Colour", QOpenGLShader::Vertex, ":/shaders/plain.vert") && success;
    success = attemptCompile(p, "Solid Colour", QOpenGLShader::Fragment, ":/shaders/solidcolor.frag") && success;
    success = attemptLink(p, "Solid Colour") && success;

    return success;
}

bool COpenGLRenderer::initialiseAttempted() const
{
    return m_bInitialiseAttempted;
}
