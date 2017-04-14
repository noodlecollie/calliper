#include "mainwindow.h"

const char* vertexShader =
        "#version 410 core\n"
        "in vec2 vPosition;\n"
        "in vec2 vTexCoord;\n"
        "out vec2 fTexCoord;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(vPosition, 0, 1);\n"
        "    fTexCoord = vTexCoord;\n"
        "}\n";

const char* fragmentShader =
        "#version 410 core\n"
        "in vec2 fTexCoord;\n"
        "out vec4 color;\n"
        "uniform sampler2D tex;\n"
        "void main()\n"
        "{\n"
        "    color = texture(tex, fTexCoord);\n"
        "}\n";

const float vertexData[] =
{
    // Positions
    -1, -1,
    1, -1,
    1, 1,
    -1, 1,

    // Texture co-ordinates
    0, 0,
    1, 0,
    1, 1,
    0, 1,
};

const unsigned int indexData[] =
{
    0, 1, 2,
    0, 2, 3,
};

MainWindow::MainWindow(QWindow *parent) :
    QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, parent),
    m_VAO(),
    m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_IndexBuffer(QOpenGLBuffer::IndexBuffer),
    m_ShaderProgram(),
    m_pTexture(Q_NULLPTR),
    m_bInitialised(false)
{
}

MainWindow::~MainWindow()
{
    makeCurrent();

    if ( m_pTexture )
    {
        m_pTexture->destroy();
        delete m_pTexture;
    }

    m_VAO.destroy();

    doneCurrent();
}

void MainWindow::initializeGL()
{
    m_VAO.create();

    m_VertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_VertexBuffer.create();
    m_VertexBuffer.bind();
    m_VertexBuffer.allocate(vertexData, sizeof(vertexData));
    m_VertexBuffer.release();

    m_IndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_IndexBuffer.create();
    m_IndexBuffer.bind();
    m_IndexBuffer.allocate(indexData, sizeof(indexData));
    m_IndexBuffer.release();

    m_pTexture = new QOpenGLTexture(QImage("/Users/Vesper/Desktop/temp.png").mirrored());

    m_ShaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
    m_ShaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);
    m_ShaderProgram.link();
    m_ShaderProgram.bind();

    m_iVertexLocation = m_ShaderProgram.attributeLocation("vPosition");
    m_iTexCoordLocation = m_ShaderProgram.attributeLocation("vTexCoord");

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
    m_pTexture->bind();

    m_ShaderProgram.enableAttributeArray(m_iVertexLocation);
    m_ShaderProgram.setAttributeBuffer(m_iVertexLocation, GL_FLOAT, 0, 2, 0);

    m_ShaderProgram.enableAttributeArray(m_iTexCoordLocation);
    m_ShaderProgram.setAttributeBuffer(m_iTexCoordLocation, GL_FLOAT, 8 * sizeof(float), 2, 0);

    context()->functions()->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    m_ShaderProgram.disableAttributeArray(m_iVertexLocation);
    m_ShaderProgram.disableAttributeArray(m_iTexCoordLocation);

    m_pTexture->release();
    m_ShaderProgram.release();
    m_VertexBuffer.release();
    m_IndexBuffer.release();
    m_VAO.release();
}
