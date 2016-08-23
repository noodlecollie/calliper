#include "shaderprogram.h"
#include <QFile>
#include <QtDebug>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_1_Core>

static const char *vertexShaderSource =
        "#version 410 core\n"
        "in vec2 vPosition;"
        "void main() { gl_Position = vec4(vPosition, -0.5, 1); }"
    ;

static const char *fragmentShaderSource =
        "#version 410 core\n"
        "out vec4 color;"
        "void main() { color = vec4(1,1,1,1); }"

    ;

ShaderProgram::ShaderProgram(const QString &name) : QObject()
{
    setObjectName(name);
    m_iShaderProgram = 0;
    m_iVertexShader = 0;
    m_iFragmentShader = 0;

    // 0xFFFFFFFF means a location is not being used.
    memset(m_iAttributeLocations, 0xFF, sizeof(GLuint)*AttributeCount);
}

ShaderProgram::~ShaderProgram()
{
    destroy();
}

void ShaderProgram::destroy()
{
    QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

    if (m_iVertexShader)
        f->glDeleteShader(m_iVertexShader);

    if (m_iFragmentShader)
        f->glDeleteShader(m_iFragmentShader);

    if (m_iShaderProgram)
        f->glDeleteProgram(m_iShaderProgram);
}

bool ShaderProgram::compileSource(ShaderType type, const char *source)
{
    switch (type)
    {
    case VertexShader:
        return compile(&m_iVertexShader, GL_VERTEX_SHADER, source);

    case FragmentShader:
        return compile(&m_iFragmentShader, GL_FRAGMENT_SHADER, source);

    default:
        return false;
    }
}

bool ShaderProgram::compile(GLuint* shaderHandle, GLuint shaderType, const char* source)
{
    QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

    *shaderHandle = f->glCreateShader(shaderType);
    if (!(*shaderHandle)) return false;

    GLint result = GL_FALSE;
    int infoLogLength;

    f->glShaderSource(*shaderHandle, 1, &source , NULL);
    f->glCompileShader(*shaderHandle);

    f->glGetShaderiv(*shaderHandle, GL_COMPILE_STATUS, &result);
    f->glGetShaderiv(*shaderHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( result != GL_TRUE )
    {
        m_LogBuffer.resize(infoLogLength);
        f->glGetShaderInfoLog(*shaderHandle, infoLogLength, NULL, m_LogBuffer.data());
        qDebug() << "Shader compilation errors:" << m_LogBuffer.constData();

        return false;
    }

    return true;
}

bool ShaderProgram::compileFile(ShaderType type, const QString &filename)
{
    QFile file(filename);
    if ( !file.open(QIODevice::ReadOnly) )
        return false;

    bool ret = compileSource(type, file.readAll().constData());
    file.close();
    return ret;
}

bool ShaderProgram::create()
{
    QOpenGLContext* c = QOpenGLContext::currentContext();
    QOpenGLFunctions_4_1_Core* f = c->versionFunctions<QOpenGLFunctions_4_1_Core>();

    m_iShaderProgram = f->glCreateProgram();
    return m_iShaderProgram != 0;
}

void ShaderProgram::link()
{
    QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

    f->glAttachShader(m_iShaderProgram, m_iVertexShader);
    f->glAttachShader(m_iShaderProgram, m_iFragmentShader);
    f->glLinkProgram(m_iShaderProgram);
}

void ShaderProgram::bind(bool b)
{
    QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

    if (b)
        f->glUseProgram(m_iShaderProgram);
    else
        f->glUseProgram(0);
}

GLuint ShaderProgram::handle() const
{
    return m_iShaderProgram;
}

void ShaderProgram::setAttributeFormat(Attribute att, int components, int strideBytes, int offsetBytes)
{
    if ( m_iAttributeLocations[att] == 0xFFFFFFFF )
        return;

    QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

    f->glVertexAttribPointer(
       m_iAttributeLocations[att],
       components,
       GL_FLOAT,
       GL_FALSE,
       strideBytes,
       reinterpret_cast<void*>(offsetBytes)
    );
}

void ShaderProgram::setUniformMatrix4(Attribute att, const QMatrix4x4 &mat)
{
    if ( m_iAttributeLocations[att] == 0xFFFFFFFF )
        return;

    QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

    f->glUniformMatrix4fv(m_iAttributeLocations[att], 1, GL_FALSE, mat.constData());
}

void ShaderProgram::setUniformVector3(Attribute att, const QVector3D &vec)
{
    if ( m_iAttributeLocations[att] == 0xFFFFFFFF )
        return;

    QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

    f->glUniform3f(m_iAttributeLocations[att], vec.x(), vec.y(), vec.z());
}

void ShaderProgram::setUniformFloat(Attribute att, float val)
{
    if ( m_iAttributeLocations[att] == 0xFFFFFFFF )
        return;

    QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

    f->glUniform1f(m_iAttributeLocations[att], val);
}

void ShaderProgram::setUniformColor4(Attribute att, const QColor &col)
{
    if ( m_iAttributeLocations[att] == 0xFFFFFFFF )
        return;

    QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

    f->glUniform4f(m_iAttributeLocations[att], col.redF(), col.greenF(), col.blueF(), col.alphaF());
}

PerVertexColorShader::PerVertexColorShader() : ShaderProgram(PerVertexColorShader::staticName())
{

}

QString PerVertexColorShader::staticName()
{
    return QString("PerVertexColorShader");
}

void PerVertexColorShader::construct()
{
    QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

    bool success = create();
    Q_ASSERT(success);

    success = compileSource(ShaderProgram::VertexShader, vertexShaderSource);
    Q_ASSERT(success);

    success = compileSource(ShaderProgram::FragmentShader, fragmentShaderSource);
    Q_ASSERT(success);

    link();

    m_iAttributeLocations[Position] = f->glGetAttribLocation(handle(), "vPosition");
}

void PerVertexColorShader::apply() const
{
    QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

    f->glUseProgram(handle());

    f->glEnableVertexAttribArray(m_iAttributeLocations[Position]);
}

void PerVertexColorShader::release() const
{
    QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

    f->glDisableVertexAttribArray(m_iAttributeLocations[Position]);
}
