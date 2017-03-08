#include "shaderstore.h"
#include "model/shaders/simplelitshader.h"
#include "model/shaders/unlitpervertexcolorshader.h"

namespace Model
{
    ShaderStore::ShaderStore()
    {
        memset(m_Shaders, 0, KnownShaderDefs::TOTAL_SHADERS * sizeof(Renderer::OpenGLShaderProgram*));

        addShaderProgram(new SimpleLitShader());
        addShaderProgram(new UnlitPerVertexColorShader());
    }

    ShaderStore::~ShaderStore()
    {
        for ( int i = 0; i < KnownShaderDefs::TOTAL_SHADERS; ++i )
        {
            delete m_Shaders[i];
        }
    }

    Renderer::OpenGLShaderProgram* ShaderStore::shaderProgram(KnownShaderDefs::KnownShaderId shaderId) const
    {
        return m_Shaders[shaderId];
    }

    Renderer::OpenGLShaderProgram* ShaderStore::operator ()(quint16 shaderId) const
    {
        if ( shaderId == KnownShaderDefs::UnknownShaderId || shaderId >= KnownShaderDefs::TOTAL_SHADERS )
        {
            return Q_NULLPTR;
        }

        return shaderProgram(static_cast<KnownShaderDefs::KnownShaderId>(shaderId));
    }

    void ShaderStore::addShaderProgram(Renderer::OpenGLShaderProgram *program)
    {
        program->construct();
        m_Shaders[program->shaderStoreId()] = program;
    }
}
