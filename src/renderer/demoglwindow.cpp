#include "demoglwindow.h"
#include <QScreen>

static const char *vertexShaderSource =
        "attribute highp vec2 vPosition;"
        "void main() { gl_Position = vec4(vPosition, 0, 1); }"
    ;

static const char *fragmentShaderSource =
        "void main() { gl_FragColor = vec4(1,1,1,1); }"
    ;

DemoGLWindow::DemoGLWindow()
{
}

DemoGLWindow::~DemoGLWindow()
{
}

void DemoGLWindow::initializeGL()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("vPosition");
    m_colAttr = m_program->attributeLocation("vColor");
    m_nrmAttr = m_program->attributeLocation("vNormal");

    m_modelWorldUniform = m_program->uniformLocation("modelToWorld");
    m_worldCameraUniform = m_program->uniformLocation("worldToCamera");
    m_coordTransformUniform = m_program->uniformLocation("hammerToOpenGL");
    m_projectionUniform = m_program->uniformLocation("projection");

    GLfloat atts[] = {
        -1,-1,
        1,-1,
        0,1,
    };

    m_pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_pVertexBuffer->create();
    m_pVertexBuffer->bind();
    m_pVertexBuffer->allocate(atts, 6*sizeof(GLfloat));

    GLushort indices[] =
    {
        0,1,2
    };

    m_pIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_pIndexBuffer->create();
    m_pIndexBuffer->bind();
    m_pIndexBuffer->allocate(indices, 3*sizeof(GLushort));
}

void DemoGLWindow::resizeGL(int w, int h)
{
    QOpenGLWindow::resizeGL(w,h);
}

void DemoGLWindow::paintGL()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (void*)0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(2);

    m_program->release();
}
