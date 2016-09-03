#include "openglshaderprogram.h"
#include "shaders/shaderdefs.h"

namespace NS_RENDERER
{
    OpenGLShaderProgram::OpenGLShaderProgram(const QString &name, QObject* parent) : QOpenGLShaderProgram(parent),
        m_iShaderStoreId(0)
    {
        setObjectName(name);
    }

    OpenGLShaderProgram::~OpenGLShaderProgram()
    {

    }

    void OpenGLShaderProgram::enableAttributeArrays()
    {
        if ( positionComponents() > 0 )
        {
            enableAttributeArray(ShaderDefs::PositionAttribute);
        }

        if ( normalComponents() > 0 )
        {
            enableAttributeArray(ShaderDefs::NormalAttribute);
        }

        if ( colorComponents() > 0 )
        {
            enableAttributeArray(ShaderDefs::ColorAttribute);
        }

        if ( textureCoordinateComponents() > 0 )
        {
            enableAttributeArray(ShaderDefs::TextureCoordinateAttribute);
        }
    }

    void OpenGLShaderProgram::disableAttributeArrays()
    {
        if ( positionComponents() > 0 )
        {
            disableAttributeArray(ShaderDefs::PositionAttribute);
        }

        if ( normalComponents() > 0 )
        {
            disableAttributeArray(ShaderDefs::NormalAttribute);
        }

        if ( colorComponents() > 0 )
        {
            disableAttributeArray(ShaderDefs::ColorAttribute);
        }

        if ( textureCoordinateComponents() > 0 )
        {
            disableAttributeArray(ShaderDefs::TextureCoordinateAttribute);
        }
    }

    quint16 OpenGLShaderProgram::shaderStoreId() const
    {
        return m_iShaderStoreId;
    }
}
