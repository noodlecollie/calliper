#include "openglshaderprogram.h"

#include <QByteArray>
#include <QFile>
#include <QtDebug>

#include "rendersystem/private/shaders/common/privateshaderdefs.h"

#include "calliperutil/opengl/openglhelpers.h"
#include "calliperutil/opengl/openglerrors.h"

namespace
{
    const QString COMMON_VERTEX_HEADERS_PATH = ":/shaders/commonvertexheaders.glsl";

    QByteArray g_CommonVertexHeaders;

    const QByteArray& commonVertexHeaders()
    {
        return g_CommonVertexHeaders;
    }

    void removeLineComments(QByteArray &arr)
    {
        int beginComment = -1;

        for ( int i = 0; i < arr.length(); ++i )
        {
            if ( arr.at(i) == '/' && arr.length() > i+1 && arr.at(i+1) == '/' && beginComment < 0 )
            {
                beginComment = i;
                continue;
            }

            if ( arr.at(i) == '\n' && beginComment >= 0 )
            {
                int length = i - beginComment + 1;
                arr.remove(beginComment, length);

                i = beginComment-1;     // Incremented next loop
                beginComment = -1;
            }
        }
    }
}

OpenGLShaderProgram::OpenGLShaderProgram(const QString &name, QObject *parent)
    : QOpenGLShaderProgram(parent),
      m_iGlobalShaderBlockIndex(GL_INVALID_INDEX),
      m_iLocalShaderBlockIndex(GL_INVALID_INDEX)
{
    setObjectName(name);
    cacheCommonHeaders();
}

OpenGLShaderProgram::~OpenGLShaderProgram()
{

}

void OpenGLShaderProgram::setGlobalUniformBlockBinding()
{
    GL_CURRENT_F;

    m_iGlobalShaderBlockIndex = f->glGetUniformBlockIndex(programId(), PrivateShaderDefs::GLOBAL_UNIFORM_BLOCK_NAME);
    if ( m_iGlobalShaderBlockIndex != GL_INVALID_INDEX )
    {
        f->glUniformBlockBinding(programId(), m_iGlobalShaderBlockIndex, PrivateShaderDefs::GlobalUniformBlockBindingPoint);
    }
    else
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Global uniform block not found in shader!");
    }
}

void OpenGLShaderProgram::setLocalUniformBlockBinding()
{
    GL_CURRENT_F;

    m_iLocalShaderBlockIndex = f->glGetUniformBlockIndex(programId(), PrivateShaderDefs::LOCAL_UNIFORM_BLOCK_NAME);
    if ( m_iLocalShaderBlockIndex != GL_INVALID_INDEX )
    {
        GLTRY(f->glUniformBlockBinding(programId(), m_iLocalShaderBlockIndex, PrivateShaderDefs::LocalUniformBlockBindingPoint));
    }
    else
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Local uniform block not found in shader!");
    }
}

bool OpenGLShaderProgram::hasLocalUniformBlockBinding() const
{
    return false;
}

void OpenGLShaderProgram::cacheCommonHeaders()
{
    if ( !g_CommonVertexHeaders.isEmpty() )
        return;

    QFile headers(COMMON_VERTEX_HEADERS_PATH);
    if ( headers.open(QIODevice::ReadOnly) )
    {
        g_CommonVertexHeaders = headers.readAll();
        headers.close();

        removeLineComments(g_CommonVertexHeaders);
    }
    else
    {
        qWarning() << "Unable to obtain shader common headers!";
    }
}

bool OpenGLShaderProgram::addShaderWithCommonHeaders(QOpenGLShader::ShaderType type, const QByteArray &shader)
{
    return addShaderFromSourceCode(type, commonVertexHeaders() + shader);
}

bool OpenGLShaderProgram::addShaderWithCommonHeaders(QOpenGLShader::ShaderType type, const char *shader)
{
    return addShaderFromSourceCode(type, commonVertexHeaders() + shader);
}

bool OpenGLShaderProgram::addShaderFileWithCommonHeaders(QOpenGLShader::ShaderType type, const QString &filePath)
{
    QFile file(filePath);
    if ( !file.open(QIODevice::ReadOnly) )
        return false;

    QByteArray arr = file.readAll();
    file.close();

    return addShaderWithCommonHeaders(type, arr);
}

bool OpenGLShaderProgram::link()
{
    bool success = QOpenGLShaderProgram::link();
    Q_ASSERT_X(success, Q_FUNC_INFO, "Shader did not link correctly!");
    return success;
}

bool OpenGLShaderProgram::bindWithUniforms()
{
    bool bindSuccess = false;
    GLTRY(bindSuccess = bind());

    if ( !bindSuccess )
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Unable to bind shader program!");
        return false;
    }

    setGlobalUniformBlockBinding();
    setLocalUniformBlockBinding();

    return true;
}
