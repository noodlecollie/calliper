#include "demoglwindow.h"
#include <QScreen>
#include <glu.h>

#define GLTRY_EX(_func, _filefunc, _line) \
{ \
    {_func;} \
    QStringList errorList; \
    for (GLenum ret = glGetError(); ret != GL_NO_ERROR; ret = glGetError()) \
        errorList.append((const char*)gluErrorString(ret)); \
    if ( !errorList.isEmpty() ) { \
        qDebug() << "OpenGL errors for command '" #_func "' at" << _filefunc << _line << "-" << errorList.join(", "); \
        exit(1); \
    } \
}

#define GLTRY(_func) GLTRY_EX(_func, Q_FUNC_INFO, __LINE__)

static const char *vertexShaderSource =
        "#version 410 core\n"
        "in vec2 vPosition;"
        "void main() { gl_Position = vec4(vPosition, 0, 1); }"
    ;

static const char *fragmentShaderSource =
        "#version 410 core\n"
        "layout(location = 0) out vec4 color;"
        "void main() { color = vec4(1,1,1,1); }"

    ;

DemoGLWindow::DemoGLWindow()
{
}

DemoGLWindow::~DemoGLWindow()
{
}

void DemoGLWindow::initializeGL()
{
    GLTRY(glGenVertexArrays(1, &m_vao))
    GLTRY(glBindVertexArray(m_vao))

#if 0
    GLTRY(m_ShaderHandle = glCreateProgram())

    {
        GLTRY(m_VertexShaderHandle = glCreateShader(GL_VERTEX_SHADER))
        GLint result = GL_FALSE;
        int infoLogLength;

        GLTRY(glShaderSource(m_VertexShaderHandle, 1, &vertexShaderSource , NULL))
        GLTRY(glCompileShader(m_VertexShaderHandle))

        GLTRY(glGetShaderiv(m_VertexShaderHandle, GL_COMPILE_STATUS, &result))
        GLTRY(glGetShaderiv(m_VertexShaderHandle, GL_INFO_LOG_LENGTH, &infoLogLength))
        if ( result != GL_TRUE )
        {
            char* logBuffer = new char[infoLogLength];
            GLTRY(glGetShaderInfoLog(m_VertexShaderHandle, infoLogLength, NULL, logBuffer))
            qDebug() << "Shader compilation errors:" << logBuffer;
            delete[] logBuffer;
        }
    }

    {
        GLTRY(m_FragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER))
        GLint result = GL_FALSE;
        int infoLogLength;

        GLTRY(glShaderSource(m_FragmentShaderHandle, 1, &fragmentShaderSource , NULL))
        GLTRY(glCompileShader(m_FragmentShaderHandle))

        GLTRY(glGetShaderiv(m_FragmentShaderHandle, GL_COMPILE_STATUS, &result))
        GLTRY(glGetShaderiv(m_FragmentShaderHandle, GL_INFO_LOG_LENGTH, &infoLogLength))
        if ( result != GL_TRUE )
        {
            char* logBuffer = new char[infoLogLength];
            GLTRY(glGetShaderInfoLog(m_FragmentShaderHandle, infoLogLength, NULL, logBuffer))
            qDebug() << "Shader compilation errors:" << logBuffer;
            delete[] logBuffer;
        }
    }

    GLTRY(glAttachShader(m_ShaderHandle, m_VertexShaderHandle))
    GLTRY(glAttachShader(m_ShaderHandle, m_FragmentShaderHandle))
    GLTRY(glLinkProgram(m_ShaderHandle))

    GLTRY(m_posAttr = glGetAttribLocation(m_ShaderHandle, "vPosition"))
#endif

    m_program = new QOpenGLShaderProgram(QOpenGLContext::currentContext());
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_program->bind();
    m_posAttr = m_program->attributeLocation("vPosition");

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
    GLTRY(glViewport(0, 0, width() * retinaScale, height() * retinaScale))

    GLTRY(glClear(GL_COLOR_BUFFER_BIT))

#if 0
    GLTRY(glUseProgram(m_ShaderHandle))
    GLTRY(glEnableVertexAttribArray(0))
    GLTRY(glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0)))
#endif

    m_program->enableAttributeArray(m_posAttr);
    m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 2);

    GLTRY(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (void*)0))

#if 0
    GLTRY(glDisableVertexAttribArray(0))
#endif

    m_program->disableAttributeArray(m_posAttr);
}
