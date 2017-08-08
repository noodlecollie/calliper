#include "customopenglwidget.h"

#include "calliperutil/opengl/openglerrors.h"
#include "calliperutil/opengl/openglhelpers.h"

namespace
{
    const char* vertexShader =
            "#version 410 core\n"
            "layout (location=0) in vec2 vPosition;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(vPosition.xy, 0.0, 1.0);\n"
            "}\n"
            ;

    const char* fragmentShader =
            "#version 410 core\n"
            "out vec4 color;"
            "void main()\n"
            "{\n"
            "   color = vec4(1,0,0,1);\n"
            "}\n"
            ;

    const float vertices[] =
    {
        -1,-1,
        1,-1,
        0,1,
    };

    const quint32 indices[] =
    {
        0,1,2,
    };

    template<typename T>
    inline void deleteResources(T& pointer)
    {
        delete pointer;
        pointer = Q_NULLPTR;
    }
}

CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f),
      m_pShaderProgram(Q_NULLPTR),
      m_pVertexBuffer(Q_NULLPTR),
      m_pIndexBuffer(Q_NULLPTR),
      m_nVAOID(0),
      m_bInitialised(false)
{

}

CustomOpenGLWidget::~CustomOpenGLWidget()
{
    m_bInitialised = false;

    makeCurrent();

    deleteResources(m_pIndexBuffer);
    deleteResources(m_pVertexBuffer);
    deleteResources(m_pShaderProgram);

    GL_CURRENT_F;
    GLTRY(f->glDeleteVertexArrays(1, &m_nVAOID));
    m_nVAOID = 0;

    doneCurrent();
}

void CustomOpenGLWidget::initializeGL()
{
    GL_CURRENT_F;
    GLTRY(f->glGenVertexArrays(1, &m_nVAOID));
    GLTRY(f->glBindVertexArray(m_nVAOID));

    m_pShaderProgram = new QOpenGLShaderProgram();
    m_pShaderProgram->create();
    m_pShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
    m_pShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);
    m_pShaderProgram->link();
    m_pShaderProgram->bind();

    m_pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_pVertexBuffer->create();
    m_pVertexBuffer->bind();
    m_pVertexBuffer->allocate(vertices, 6 * sizeof(float));
    m_pVertexBuffer->release();

    m_pIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_pIndexBuffer->create();
    m_pIndexBuffer->bind();
    m_pIndexBuffer->allocate(indices, 3 * sizeof(quint32));
    m_pIndexBuffer->release();

    GLTRY(f->glBindVertexArray(0));

    m_bInitialised = true;
}

void CustomOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void CustomOpenGLWidget::paintGL()
{
    if ( !m_bInitialised )
    {
        return;
    }

    GL_CURRENT_F;
    GLTRY(f->glBindVertexArray(m_nVAOID));

    m_pShaderProgram->bind();
    m_pVertexBuffer->bind();
    m_pIndexBuffer->bind();

    m_pShaderProgram->enableAttributeArray(0);
    m_pShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 2);

    GLTRY(f->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));

    m_pShaderProgram->disableAttributeArray(0);
    m_pIndexBuffer->release();
    m_pVertexBuffer->release();
    m_pShaderProgram->release();

    GLTRY(f->glBindVertexArray(0));
}
