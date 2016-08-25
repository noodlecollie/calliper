#include "demoglwindow.h"
#include <QScreen>
#include <QOpenGLFunctions_4_1_Core>
#include "openglerrors.h"

#define GLTRY_EX(_func, _filefunc, _line) \
{ \
    {_func;} \
    QStringList errorList; \
    for (GLenum ret = QOpenGLContext::currentContext()->functions()->glGetError(); ret != GL_NO_ERROR; \
            ret = QOpenGLContext::currentContext()->functions()->glGetError()) \
        errorList.append(OpenGLErrors::errorString(ret)); \
    if ( !errorList.isEmpty() ) { \
        qDebug() << "OpenGL errors for command '" #_func "' at" << _filefunc << _line << "-" << errorList.join(", "); \
        exit(1); \
    } \
}

#define GLTRY(_func) GLTRY_EX(_func, Q_FUNC_INFO, __LINE__)

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
        QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

        GLTRY(f->glGenVertexArrays(1, &m_vao))
        GLTRY(f->glBindVertexArray(m_vao))

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

            1,0,0,1,    // 6 7 8 9
            0,1,0,1,    // 10 11 12 13
            0,0,1,1,    // 14 15 16 17
        };

        m_pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_pVertexBuffer->create();
        m_pVertexBuffer->bind();
        m_pVertexBuffer->allocate(atts, 18*sizeof(GLfloat));

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
        GLTRY(f->glViewport(0, 0, width() * retinaScale, height() * retinaScale))

        GLTRY(f->glClear(GL_COLOR_BUFFER_BIT))

        m_program->enableAttributeArray(m_posAttr);
        m_program->enableAttributeArray(m_colAttr);

        m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0*sizeof(GLfloat), 2);
        m_program->setAttributeBuffer(m_colAttr, GL_FLOAT, 6*sizeof(GLfloat), 4);

        GLTRY(f->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (void*)0))

        m_program->disableAttributeArray(m_posAttr);
        m_program->disableAttributeArray(m_colAttr);
    }
}
