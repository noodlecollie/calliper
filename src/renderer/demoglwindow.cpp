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

#define ENABLE_NEW

#ifdef ENABLE_NEW
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
#else
static const char *vertexShaderSource =
        "attribute highp vec2 vPosition;"
        "void main() { gl_Position = vec4(vPosition, 0, 1); }"
    ;

static const char *fragmentShaderSource =
        "void main() { gl_FragColor = vec4(1,1,1,1); }"
    ;
#endif

DemoGLWindow::DemoGLWindow()
{
}

DemoGLWindow::~DemoGLWindow()
{
}

void DemoGLWindow::initializeGL()
{
#ifdef ENABLE_NEW
    GLTRY(glGenVertexArrays(1, &m_vao))
    GLTRY(glBindVertexArray(m_vao))

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

#else
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
#endif

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

#ifdef ENABLE_NEW
    GLTRY(glUseProgram(m_ShaderHandle))
    GLTRY(glEnableVertexAttribArray(0))
    GLTRY(glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0)))
#else
    m_program->bind();

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);
    GLTRY(glEnableVertexAttribArray(0))
#endif

    GLTRY(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (void*)0))

#ifdef ENABLE_NEW
    GLTRY(glDisableVertexAttribArray(0))
#else
    GLTRY(glDisableVertexAttribArray(0))
    m_program->release();
#endif
}
