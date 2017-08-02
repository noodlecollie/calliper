#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QOpenGLFunctions>

#include "calliperutil/opengl/openglerrors.h"
#include "calliperutil/opengl/openglhelpers.h"
#include "calliperutil/math/math.h"

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
        1, 1,
        -1, 1,
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
    m_nVAOID(0)
{

}

MainWindow::~MainWindow()
{
    makeCurrent();

    m_bInitialised = false;
    m_RefreshTimer.stop();

    GL_CURRENT_F;

    GLTRY(f->glDeleteVertexArrays(1, &m_nVAOID));

    delete m_pCubeVertexBuffer;
    m_pCubeVertexBuffer = Q_NULLPTR;

    delete m_pCubeIndexBuffer;
    m_pCubeIndexBuffer = Q_NULLPTR;

    delete m_pQuadVertexBuffer;
    m_pQuadVertexBuffer = Q_NULLPTR;

    delete m_pQuadIndexBuffer;
    m_pQuadIndexBuffer = Q_NULLPTR;

    delete m_pCubeShaderProgram;
    m_pCubeShaderProgram = Q_NULLPTR;

    delete m_pQuadShaderProgram;
    m_pQuadShaderProgram = Q_NULLPTR;

    delete m_pFrameBuffer;
    m_pFrameBuffer = Q_NULLPTR;

    doneCurrent();
}

void MainWindow::initializeGL()
{
    m_pFrameBuffer = new QOpenGLFramebufferObject(size());

    GL_CURRENT_F;

    GLTRY(f->glEnable(GL_DEPTH_TEST));
    GLTRY(f->glDepthFunc(GL_LEQUAL));

    GLTRY(f->glGenVertexArrays(1, &m_nVAOID));
    GLTRY(f->glBindVertexArray(m_nVAOID));

    m_pCubeShaderProgram = new QOpenGLShaderProgram();
    m_pCubeShaderProgram->create();
    m_pCubeShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, cubeVertexShader);
    m_pCubeShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, cubeFragmentShader);
    m_pCubeShaderProgram->link();

    m_nModelToWorldMatrixLocation = m_pCubeShaderProgram->uniformLocation("modelToWorldMatrix");

    m_pQuadShaderProgram = new QOpenGLShaderProgram();
    m_pQuadShaderProgram->create();
    m_pQuadShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, quadVertexShader);
    m_pQuadShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, quadFragmentShader);
    m_pQuadShaderProgram->link();

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

void MainWindow::resizeGL(int w, int h)
{
    if ( !m_bInitialised || !context() || QOpenGLContext::currentContext() != context() )
    {
        return;
    }

    delete m_pFrameBuffer;
    m_pFrameBuffer = new QOpenGLFramebufferObject(QSize(w,h));
}

void MainWindow::paintGL()
{
    if ( !m_bInitialised )
    {
        return;
    }

    GL_CURRENT_F;

    GLTRY(f->glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

    GLTRY(f->glBindVertexArray(m_nVAOID));

    // TODO:
    // - Bind FBO
    // - Render cube to FBO
    // - Release FBO
    // - Take FBO texture
    // - Render quad with texture

    drawCube();

    GLTRY(f->glBindVertexArray(0));

    ++m_nFrames;
}

void MainWindow::drawCube()
{
    QMatrix4x4 modelToWorld;
    modelToWorld = CalliperUtil::Math::matrixRotateY((float)m_nFrames / 10.0f) * modelToWorld;
    modelToWorld = CalliperUtil::Math::matrixRotateX(-0.25f) * modelToWorld;
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
