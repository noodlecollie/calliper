#include "mainwindow.h"

const char* vertexShader =
        "#version 410 core\n"
        "in vec2 vPosition;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(vPosition, 0, 1);\n"
        "}\n";

const char* fragmentShader =
        "#version 410 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "    color = vec4(1,0,0,1);\n"
        "}\n";

const float vertexData[] =
{
    -1, -1,
    1, -1,
    0, 1,
};

const unsigned int indexData[] =
{
    0, 1, 2,
};

MainWindow::MainWindow(QWindow *parent) :
    QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, parent),
    m_VAO(),
    m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_IndexBuffer(QOpenGLBuffer::IndexBuffer),
    m_ShaderProgram(),
    m_bInitialised(false)
{
}

MainWindow::~MainWindow()
{
    makeCurrent();

    m_VAO.destroy();

    doneCurrent();
}

void MainWindow::initializeGL()
{
    QOpenGLContext* currentContext = QOpenGLContext::currentContext();
    m_VAO.create();

    m_VertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_VertexBuffer.create();
    m_VertexBuffer.bind();
    m_VertexBuffer.allocate(vertexData, 6 * sizeof(float));
    m_VertexBuffer.release();

    m_IndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_IndexBuffer.create();
    m_IndexBuffer.bind();
    m_IndexBuffer.allocate(indexData, 3 * sizeof(unsigned int));
    m_IndexBuffer.release();

    m_ShaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
    m_ShaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);
    m_ShaderProgram.link();
    m_ShaderProgram.bind();

    m_iVertexLocation = m_ShaderProgram.attributeLocation("vPosition");

    m_ShaderProgram.release();

    m_bInitialised = true;
}

void MainWindow::resizeGL(int w, int h)
{
    if ( !m_bInitialised )
    {
        return;
    }
}

void MainWindow::paintGL()
{
    if ( !m_bInitialised )
    {
        return;
    }

    m_VAO.bind();
    m_VertexBuffer.bind();
    m_IndexBuffer.bind();
    m_ShaderProgram.bind();

    m_ShaderProgram.enableAttributeArray(m_iVertexLocation);
    m_ShaderProgram.setAttributeBuffer(m_iVertexLocation, GL_FLOAT, 0, 2, 0);
    context()->functions()->glDrawArrays(GL_TRIANGLES, 0, 3);
    m_ShaderProgram.disableAttributeArray(m_iVertexLocation);

    m_VertexBuffer.release();
    m_IndexBuffer.release();
    m_VAO.release();
}
