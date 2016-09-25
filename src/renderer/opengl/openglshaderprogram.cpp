#include "openglshaderprogram.h"
#include "shaders/shaderdefs.h"
#include "opengl/openglhelpers.h"

namespace NS_RENDERER
{
    OpenGLShaderProgram::OpenGLShaderProgram(quint16 id, const QString &name, QObject* parent) : QOpenGLShaderProgram(parent),
        m_iShaderStoreId(id), m_iGlobalShaderBlockIndex(GL_INVALID_INDEX)
    {
        Q_ASSERT_X(m_iShaderStoreId > 0, Q_FUNC_INFO, "ShaderID should not be zero!");
        setObjectName(name);
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
        // Leave this to subclasses.
    }
}
