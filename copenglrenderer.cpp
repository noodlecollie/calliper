#include "copenglrenderer.h"
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QGuiApplication>
#include <QOpenGLShaderProgram>
#include <QtDebug>
#include <QMetaEnum>

#include <QOpenGLFunctions_3_2_Core>
#include "csceneobject.h"
#include "cbasiccamera.h"
#include <QMatrix4x4>
#include "cvertexbundle.h"
#include "ccameralens.h"
#include <QSurface>

// Remove me
#include "cdebugtriangle.h"
CDebugTriangle* dbgt = NULL;

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

bool attemptCompile(QOpenGLShaderProgram* program, QOpenGLShader::ShaderType type, const QString &path)
{
    if ( !program->addShaderFromSourceFile(type, path) )
    {
        qCritical().nospace() << "Compilation failed for " << program->objectName() << " with shader file " << path << ": " << program->log();
        return false;
    }

    return true;
}

bool attemptLink(QOpenGLShaderProgram* program)
{
    if ( !program->link() )
    {
        qCritical().nospace() << "Linking failed for shader " << program->objectName() << ": " << program->log();
        return false;
    }

    return true;
}

COpenGLRenderer::COpenGLRenderer(QObject *parent) : QObject(parent)
{
    m_pBackgroundContext = NULL;
    m_bInitialised = false;
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

void COpenGLRenderer::initialise(QSurface* surface)
{
    switch (0)
    {
        default:
        if ( !createBackgroundContext() ) break;

        m_pBackgroundContext->makeCurrent(surface);
        if ( !compileShaders() ) break;

        dbgt = new CDebugTriangle();
        m_pBackgroundContext->doneCurrent();
        m_bInitialised = true;
        return;
    }

    m_bInitialised = false;
}

bool COpenGLRenderer::isValid() const
{
    return m_pBackgroundContext && m_bInitialised && m_pBackgroundContext->isValid();
}

bool COpenGLRenderer::compileShaders()
{
    // Solid colour
    QOpenGLShaderProgram* p = new QOpenGLShaderProgram(m_pBackgroundContext);
    p->setObjectName("Solid Colour");
    m_ShaderList.append(p);

    if ( !attemptCompile(p, QOpenGLShader::Vertex, ":/shaders/plain.vert") ) return false;
    if ( !attemptCompile(p, QOpenGLShader::Fragment, ":/shaders/solidcolor.frag") ) return false;
    if ( !attemptLink(p) ) return false;

    return true;
}

void COpenGLRenderer::render(QOpenGLFunctions_3_2_Core *f, const CSceneObject *root, const CBasicCamera* camera)
{
    /*
    QMatrix4x4 projection = camera->lens()->projectionMatrix();
    QMatrix4x4 view = camera->worldToCamera();
    QMatrix4x4 modelViewProjection = projection * CBasicCamera::coordsHammerToOpenGL() * view;


    // Bind the vertex buffer we want to use.
    CVertexBundle* v = root->vertexData();
    v->bindVertexBuffer(true);
    QOpenGLShaderProgram* p = m_ShaderList.first();

    p->bind();

    p->setAttributeBuffer("vec_Position",
                          GL_FLOAT,
                          COpenGLRenderer::attributeOffset(COpenGLRenderer::FormatPositionUV, COpenGLRenderer::Position),
                          COpenGLRenderer::attributeSize(COpenGLRenderer::Position)/sizeof(float),
                          COpenGLRenderer::interleavingFormatSize(COpenGLRenderer::FormatPositionUV));
    p->setUniformValue("mat_MVP", modelViewProjection);

    v->bindIndexBuffer(true);
    f->glDrawElements(GL_TRIANGLES, v->indexCount(), GL_UNSIGNED_INT, (void*)0);

    p->release();
    */
    
    QOpenGLShaderProgram* p = m_ShaderList.first();
    CVertexBundle* v = dbgt->vertexData();
    p->bind();
    v->bindVertexBuffer(true);
    p->setAttributeBuffer("vec_Position",
                          GL_FLOAT,
                          COpenGLRenderer::attributeOffset(v->interleavingFormat(), COpenGLRenderer::Position),
                          COpenGLRenderer::attributeSize(COpenGLRenderer::Position)/sizeof(float),
                          COpenGLRenderer::interleavingFormatSize(v->interleavingFormat()));
    p->setUniformValue("mat_MVP", QMatrix4x4());
    p->setUniformValue("vec_Color", QVector4D(1,0,0,1));
    p->enableAttributeArray("vec_Position");
    v->bindIndexBuffer(true);
    f->glDrawElements(GL_TRIANGLES, v->indexCount(), GL_UNSIGNED_INT, (void*)0);
    p->release();
}
