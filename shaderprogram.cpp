#include "shaderprogram.h"
#include "resourcemanager.h"
#include <QFile>
#include <QtDebug>
#include <QMatrix4x4>

ShaderProgram::ShaderProgram(const QString &name) : QObject()
{
    setObjectName(name);
    m_iShaderProgram = 0;
    m_iVertexShader = 0;
    m_iFragmentShader = 0;

    memset(m_iAttributeLocations, 0xFF, sizeof(GLuint)*AttributeCount);
}

ShaderProgram::~ShaderProgram()
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

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
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

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
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    m_iShaderProgram = f->glCreateProgram();
    return m_iShaderProgram != 0;
}

void ShaderProgram::link()
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glAttachShader(m_iShaderProgram, m_iVertexShader);
    f->glAttachShader(m_iShaderProgram, m_iFragmentShader);
    f->glLinkProgram(m_iShaderProgram);
}

void ShaderProgram::bind(bool b)
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

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

    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glVertexAttribPointer(
       m_iAttributeLocations[att],
       components,
       GL_FLOAT,
       GL_FALSE,
       strideBytes,
       (void*)offsetBytes
    );
}

void ShaderProgram::setUniformMatrix4(Attribute att, const QMatrix4x4 &mat)
{
    if ( m_iAttributeLocations[att] == 0xFFFFFFFF )
        return;

    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glUniformMatrix4fv(m_iAttributeLocations[att], 1, GL_FALSE, mat.constData());
}
