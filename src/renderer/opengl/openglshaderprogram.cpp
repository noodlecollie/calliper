#include "openglshaderprogram.h"
#include "shaders/shaderdefs.h"
#include "opengl/openglhelpers.h"
#include "opengl/openglerrors.h"
#include <QByteArray>
#include <QFile>
#include <QtDebug>

namespace
{
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

namespace NS_RENDERER
{
    OpenGLShaderProgram::OpenGLShaderProgram(quint16 id, const QString &name, QObject* parent) : QOpenGLShaderProgram(parent),
        m_iShaderStoreId(id), m_iGlobalShaderBlockIndex(GL_INVALID_INDEX), m_iLocalShaderBlockIndex(GL_INVALID_INDEX)
    {
        Q_ASSERT_X(m_iShaderStoreId > 0, Q_FUNC_INFO, "ShaderID should not be zero!");
        setObjectName(name);

        cacheCommonHeaders();
    }

    OpenGLShaderProgram::~OpenGLShaderProgram()
    {

    }

    void OpenGLShaderProgram::enableAttributeArrays()
    {
        VertexFormat format = vertexFormat();

        if ( format.positionComponents() > 0 )
        {
            enableAttributeArray(ShaderDefs::PositionAttribute);
        }

        if ( format.normalComponents() > 0 )
        {
            enableAttributeArray(ShaderDefs::NormalAttribute);
        }

        if ( format.colorComponents() > 0 )
        {
            enableAttributeArray(ShaderDefs::ColorAttribute);
        }

        if ( format.textureCoordinateComponents() > 0 )
        {
            enableAttributeArray(ShaderDefs::TextureCoordinateAttribute);
        }
    }

    void OpenGLShaderProgram::disableAttributeArrays()
    {
        VertexFormat format = vertexFormat();

        if ( format.positionComponents() > 0 )
        {
            disableAttributeArray(ShaderDefs::PositionAttribute);
        }

        if ( format.normalComponents() > 0 )
        {
            disableAttributeArray(ShaderDefs::NormalAttribute);
        }

        if ( format.colorComponents() > 0 )
        {
            disableAttributeArray(ShaderDefs::ColorAttribute);
        }

        if ( format.textureCoordinateComponents() > 0 )
        {
            disableAttributeArray(ShaderDefs::TextureCoordinateAttribute);
        }
    }

    quint16 OpenGLShaderProgram::shaderStoreId() const
    {
        return m_iShaderStoreId;
    }

    void OpenGLShaderProgram::setGlobalUniformBlockBinding()
    {
        GL_CURRENT_F;

        m_iGlobalShaderBlockIndex = f->glGetUniformBlockIndex(programId(), ShaderDefs::GLOBAL_UNIFORM_BLOCK_NAME);
        if ( m_iGlobalShaderBlockIndex != GL_INVALID_INDEX )
        {
            f->glUniformBlockBinding(programId(), m_iGlobalShaderBlockIndex, ShaderDefs::GlobalUniformBlockBindingPoint);
        }
        else
        {
            Q_ASSERT_X(false, Q_FUNC_INFO, "Global uniform block not found in shader!");
        }
    }

    void OpenGLShaderProgram::setLocalUniformBlockBinding()
    {
        GL_CURRENT_F;

        m_iLocalShaderBlockIndex = f->glGetUniformBlockIndex(programId(), ShaderDefs::LOCAL_UNIFORM_BLOCK_NAME);
        if ( m_iLocalShaderBlockIndex != GL_INVALID_INDEX )
        {
            GLTRY(f->glUniformBlockBinding(programId(), m_iLocalShaderBlockIndex, ShaderDefs::LocalUniformBlockBindingPoint));
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

        QFile headers(":/renderer/shaders/commonvertexheaders.glsl");
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

    bool OpenGLShaderProgram::addVertexShaderWithCommonHeaders(const QByteArray &shader)
    {
        return addShaderFromSourceCode(QOpenGLShader::Vertex, commonVertexHeaders() + shader);
    }

    bool OpenGLShaderProgram::addVertexShaderWithCommonHeaders(const char *shader)
    {
        return addShaderFromSourceCode(QOpenGLShader::Vertex, commonVertexHeaders() + shader);
    }

    bool OpenGLShaderProgram::addVertexShaderFileWithCommonHeaders(const QString &filePath)
    {
        QFile file(filePath);
        if ( !file.open(QIODevice::ReadOnly) )
            return false;

        QByteArray arr = file.readAll();
        file.close();

        return addVertexShaderWithCommonHeaders(arr);
    }
}
