#include "mainwindow.h"

#include <QOpenGLFunctions>
#include <QGuiApplication>
#include <QOpenGLFramebufferObjectFormat>

#include "calliperutil/opengl/openglerrors.h"
#include "calliperutil/opengl/openglhelpers.h"
#include "calliperutil/math/math.h"

#define USE_FRAMEBUFFER

namespace
{
    const float cubeVerts[] =
    {
        -1,-1,-1,   // 0
         1,-1,-1,   // 1
        -1, 1,-1,   // 2
         1, 1,-1,   // 3
        -1,-1, 1,   // 4
         1,-1, 1,   // 5
        -1, 1, 1,   // 6
         1, 1, 1,   // 7
    };

    const quint32 cubeIndices[] =
    {
        0,1,3,
        0,3,2,

        5,4,6,
        5,6,7,

        1,5,7,
        1,7,3,

        4,0,2,
        4,2,6,

        2,3,7,
        2,7,6,

        0,5,1,
        0,4,5,
    };

    const float quadVerts[] =
    {
        -1, -1,
         1, -1,
         1,  1,
        -1,  1,
    };

    const quint32 quadIndices[] =
    {
        0,1,2,
        0,2,3,
    };

    const char* cubeVertexShader =
            "#version 410 core\n"
            "layout (location=0) in vec3 vPosition;\n"
            "out vec4 fColour;\n"
            "uniform mat4 modelToWorldMatrix;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = modelToWorldMatrix * vec4(vPosition.xyz, 1.0);\n"
            "   fColour = vec4((vPosition.xyz + vec3(1,1,1)) / 2.0, 1);\n"
            "}\n"
            ;

    const char* cubeFragmentShader =
            "#version 410 core\n"
            "in vec4 fColour;\n"
            "layout(location = 0) out vec4 color;\n"
            "void main()\n"
            "{\n"
            "   color = fColour;\n"
            "}\n"
            ;

    const char* quadVertexShader =
            "#version 410 core\n"
            "layout (location=0) in vec2 vPosition;\n"
            "out vec2 fPosition;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(vPosition.xy, 0.0, 1.0);\n"
            "   fPosition = (vPosition + vec2(1,1)) / 2.0;\n"
            "}\n"
            ;

    const char* quadFragmentShader =
            "#version 410 core\n"
            "in vec2 fPosition;\n"
            "layout(location = 0) out vec4 color;\n"
            "uniform sampler2D tex;\n"
            "void main()\n"
            "{\n"
            "   color = texture(tex, fPosition);\n"
            "}\n"
            ;
}

MainWindow::MainWindow() :
    QOpenGLWindow(),
    m_pFrameBuffer(Q_NULLPTR),
    m_pCubeShaderProgram(Q_NULLPTR),
    m_pQuadShaderProgram(Q_NULLPTR),
    m_pCubeVertexBuffer(Q_NULLPTR),
    m_pCubeIndexBuffer(Q_NULLPTR),
    m_pQuadVertexBuffer(Q_NULLPTR),
    m_pQuadIndexBuffer(Q_NULLPTR),
    m_RefreshTimer(),
    m_bInitialised(false),
    m_nFrames(0),
    m_nVAOID(0),
    m_nModelToWorldMatrixLocation(0),
    m_pSeparateContext(Q_NULLPTR),
    m_pOffscreenSurface(Q_NULLPTR),
    m_nCubeVAOID(0)
{
    m_pSeparateContext = new QOpenGLContext();
    m_pSeparateContext->setFormat(QSurfaceFormat::defaultFormat());

    QOpenGLContext* globalShareContext = QOpenGLContext::globalShareContext();
    Q_ASSERT(globalShareContext);
    m_pSeparateContext->setShareContext(globalShareContext);
    m_pSeparateContext->create();

    m_pOffscreenSurface = new QOffscreenSurface();
    m_pOffscreenSurface->setFormat(QSurfaceFormat::defaultFormat());
    m_pOffscreenSurface->create();
}

MainWindow::~MainWindow()
{
    m_bInitialised = false;
    m_RefreshTimer.stop();

    {
        makeCurrent();

        GL_CURRENT_F;
        GLTRY(f->glDeleteVertexArrays(1, &m_nVAOID));

        delete m_pQuadVertexBuffer;
        m_pQuadVertexBuffer = Q_NULLPTR;

        delete m_pQuadIndexBuffer;
        m_pQuadIndexBuffer = Q_NULLPTR;

        delete m_pQuadShaderProgram;
        m_pQuadShaderProgram = Q_NULLPTR;

        doneCurrent();
    }

    {
        makeCurrentInternal();

        delete m_pFrameBuffer;
        m_pFrameBuffer = Q_NULLPTR;

        delete m_pCubeVertexBuffer;
        m_pCubeVertexBuffer = Q_NULLPTR;

        delete m_pCubeIndexBuffer;
        m_pCubeIndexBuffer = Q_NULLPTR;

        delete m_pCubeShaderProgram;
        m_pCubeShaderProgram = Q_NULLPTR;

        doneCurrentInternal();
    }

    delete m_pSeparateContext;
    m_pSeparateContext = Q_NULLPTR;

    delete m_pOffscreenSurface;
    m_pOffscreenSurface = Q_NULLPTR;
}

void MainWindow::initializeGL()
{
    {
        makeCurrentInternal();

        generateFrameBufferObject();
        m_pFrameBuffer->bind();
        setOpenGLOptionsForBoundFrameBuffer();
        m_pFrameBuffer->release();

        GL_CURRENT_F;
        GLTRY(f->glGenVertexArrays(1, &m_nCubeVAOID));
        GLTRY(f->glBindVertexArray(m_nCubeVAOID));

        m_pCubeShaderProgram = new QOpenGLShaderProgram();
        m_pCubeShaderProgram->create();
        m_pCubeShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, cubeVertexShader);
        m_pCubeShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, cubeFragmentShader);
        m_pCubeShaderProgram->link();

        m_nModelToWorldMatrixLocation = m_pCubeShaderProgram->uniformLocation("modelToWorldMatrix");

        m_pCubeVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_pCubeVertexBuffer->create();
        m_pCubeVertexBuffer->bind();
        m_pCubeVertexBuffer->allocate(cubeVerts, 8 * 3 * sizeof(float));
        m_pCubeVertexBuffer->release();

        m_pCubeIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
        m_pCubeIndexBuffer->create();
        m_pCubeIndexBuffer->bind();
        m_pCubeIndexBuffer->allocate(cubeIndices, 3 * 12 * sizeof(quint32));
        m_pCubeIndexBuffer->release();

        GLTRY(f->glBindVertexArray(0));

        makeCurrent();
    }

    setOpenGLOptionsForBoundFrameBuffer();

    GL_CURRENT_F;
    GLTRY(f->glGenVertexArrays(1, &m_nVAOID));
    GLTRY(f->glBindVertexArray(m_nVAOID));

    m_pQuadShaderProgram = new QOpenGLShaderProgram();
    m_pQuadShaderProgram->create();
    m_pQuadShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, quadVertexShader);
    m_pQuadShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, quadFragmentShader);
    m_pQuadShaderProgram->link();

    m_pQuadVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_pQuadVertexBuffer->create();
    m_pQuadVertexBuffer->bind();
    m_pQuadVertexBuffer->allocate(quadVerts, 2 * 4 * sizeof(float));
    m_pQuadVertexBuffer->release();

    m_pQuadIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_pQuadIndexBuffer->create();
    m_pQuadIndexBuffer->bind();
    m_pQuadIndexBuffer->allocate(quadIndices, 3 * 2 * sizeof(quint32));

    GLTRY(f->glBindVertexArray(0));

    m_RefreshTimer.setInterval((int)(1000.0f / 60.0f));
    connect(&m_RefreshTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_RefreshTimer.start();

    m_bInitialised = true;
}

void MainWindow::setOpenGLOptionsForBoundFrameBuffer()
{
    GL_CURRENT_F;

    GLTRY(f->glEnable(GL_DEPTH_TEST));
    GLTRY(f->glDepthFunc(GL_LEQUAL));
    GLTRY(f->glClearDepth(1.0f));
    GLTRY(f->glCullFace(GL_BACK));
    GLTRY(f->glFrontFace(GL_CCW));
}

void MainWindow::generateFrameBufferObject()
{
    QOpenGLFramebufferObjectFormat fboFormat;
    fboFormat.setAttachment(QOpenGLFramebufferObject::Depth);
    fboFormat.setTextureTarget(GL_TEXTURE_2D);

    m_pFrameBuffer = new QOpenGLFramebufferObject(size(), fboFormat);
}

void MainWindow::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);

    if ( !m_bInitialised || !context() || QOpenGLContext::currentContext() != context() )
    {
        return;
    }

    {
        makeCurrentInternal();

        delete m_pFrameBuffer;
        generateFrameBufferObject();

        makeCurrent();
    }
}

void MainWindow::paintGL()
{
    if ( !m_bInitialised )
    {
        return;
    }

    {
        makeCurrentInternal();

        GL_CURRENT_F;
        GLTRY(f->glBindVertexArray(m_nCubeVAOID));

        m_pFrameBuffer->bind();
        GLTRY(f->glViewport(0, 0, m_pFrameBuffer->width(), m_pFrameBuffer->height()));

        GLTRY(f->glClearColor(0.4f, 0.4f, 0.4f, 1.0f));
        GLTRY(f->glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
        drawCube();

        m_pFrameBuffer->release();
        GLTRY(f->glBindVertexArray(0));

        makeCurrent();
    }

    GL_CURRENT_F;
    GLTRY(f->glFinish());

    GLTRY(f->glBindVertexArray(m_nVAOID));

    const int pixelRatio = (int)qApp->devicePixelRatio();
    GLTRY(f->glViewport(0, 0, size().width() * pixelRatio, size().height() * pixelRatio));
    GLTRY(f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLTRY(f->glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
    drawQuad();

    GLTRY(f->glBindVertexArray(0));

    ++m_nFrames;
}

void MainWindow::drawCube()
{
    GL_CURRENT_F;

    const float sinFrames = qSin((float)m_nFrames / 15.0f);

    QMatrix4x4 modelToWorld;
    modelToWorld = CalliperUtil::Math::matrixRotateY((float)m_nFrames / 10.0f) * modelToWorld;
    modelToWorld = CalliperUtil::Math::matrixRotateX(0.25f * sinFrames) * modelToWorld;
    modelToWorld = CalliperUtil::Math::matrixScaleUniform(0.5f) * modelToWorld;

    m_pCubeShaderProgram->bind();
    m_pCubeVertexBuffer->bind();
    m_pCubeIndexBuffer->bind();

    m_pCubeShaderProgram->enableAttributeArray(0);
    m_pCubeShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    m_pCubeShaderProgram->setUniformValue(m_nModelToWorldMatrixLocation, modelToWorld);

    GLTRY(f->glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));

    m_pCubeShaderProgram->disableAttributeArray(0);

    m_pCubeIndexBuffer->release();
    m_pCubeVertexBuffer->release();
    m_pCubeShaderProgram->release();
}

void MainWindow::drawQuad()
{
    GL_CURRENT_F;

    m_pQuadShaderProgram->bind();
    m_pQuadVertexBuffer->bind();
    m_pQuadIndexBuffer->bind();

    m_pQuadShaderProgram->enableAttributeArray(0);
    m_pQuadShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 2);

    GLTRY(f->glBindTexture(GL_TEXTURE_2D, m_pFrameBuffer->texture()));

    GLTRY(f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    GLTRY(f->glBindTexture(GL_TEXTURE_2D, 0));

    m_pQuadIndexBuffer->release();
    m_pQuadVertexBuffer->release();
    m_pQuadShaderProgram->release();
}

void MainWindow::makeCurrentInternal()
{
    m_pSeparateContext->makeCurrent(m_pOffscreenSurface);
}

void MainWindow::doneCurrentInternal()
{
    m_pSeparateContext->doneCurrent();
}
