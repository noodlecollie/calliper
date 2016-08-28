#include "demoglwindow.h"
#include <QScreen>
#include <QOpenGLFunctions_4_1_Core>
#include "openglerrors.h"
#include <QtDebug>

namespace NS_RENDERER
{
    static const char *vertexShaderSource =
            "#version 410 core\n"
            "in vec2 vPosition;"
            "in vec4 vColour;"
            "out vec4 fColour;"
            "void main() { gl_Position = vec4(vPosition, 0, 1); fColour = vColour; }"
        ;

    static const char *fragmentShaderSource =
            "#version 410 core\n"
            "in vec4 fColour;"
            "layout(location = 0) out vec4 color;"
            "void main() { color = fColour; }"
        ;

    DemoGLWindow::DemoGLWindow()
    {
    }

    DemoGLWindow::~DemoGLWindow()
    {
    }

    void DemoGLWindow::initializeGL()
    {
        qDebug() << OpenGLErrors::debugOpenGLCapabilities().toLatin1().constData();
        QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

        GLTRY(f->glGenVertexArrays(1, &m_vao));
        GLTRY(f->glBindVertexArray(m_vao));

        m_program = new QOpenGLShaderProgram(QOpenGLContext::currentContext());
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
        m_program->link();
        m_program->bind();
        m_posAttr = m_program->attributeLocation("vPosition");
        m_colAttr = m_program->attributeLocation("vColour");

        GLfloat atts[] = {
            -1,-1,      // 0 1
            1,-1,       // 2 3
            0,1,        // 4 5
        };

        m_pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_pVertexBuffer->create();
        m_pVertexBuffer->bind();
        m_pVertexBuffer->allocate(atts, 6*sizeof(GLfloat));

        GLfloat cols[] = {
            1,0,0,1,
            0,1,0,1,
            0,0,1,1,
        };

        m_pColourBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_pColourBuffer->create();
        m_pColourBuffer->bind();
        m_pColourBuffer->allocate(cols, 12*sizeof(GLfloat));

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
        QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

        const qreal retinaScale = devicePixelRatio();
        GLTRY(f->glViewport(0, 0, width() * retinaScale, height() * retinaScale));

        GLTRY(f->glClear(GL_COLOR_BUFFER_BIT));

        m_program->enableAttributeArray(m_posAttr);
        m_pVertexBuffer->bind();
        m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0*sizeof(GLfloat), 2);

        m_program->enableAttributeArray(m_colAttr);
        m_pColourBuffer->bind();
        m_program->setAttributeBuffer(m_colAttr, GL_FLOAT, 0*sizeof(GLfloat), 4);

        GLTRY(f->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (void*)0));

        m_program->disableAttributeArray(m_posAttr);
        m_program->disableAttributeArray(m_colAttr);
    }
}
