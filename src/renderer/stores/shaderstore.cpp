#include "stores/shaderstore.h"
#include "shaders/debugscreenspaceshader.h"
#include "opengl/openglhelpers.h"
#include "shaders/shaderdefs.h"

namespace NS_RENDERER
{
    ShaderStore* g_pGlobalShaderStore = NULL;

    ShaderStore* ShaderStore::getShaderStore()
    {
        return g_pGlobalShaderStore;
    }

    ShaderStore::ShaderStore()
    {
        Q_ASSERT_X(!g_pGlobalShaderStore, Q_FUNC_INFO, "Shader store singleton already created!");

        g_pGlobalShaderStore = this;
        addBuiltInShaders();
    }

    ShaderStore::~ShaderStore()
    {
        m_ShaderNameTable.clear();
        qDeleteAll(m_ShaderList);

        g_pGlobalShaderStore = NULL;
    }

    void ShaderStore::addBuiltInShaders()
    {
        addShader(new DebugScreenSpaceShader());
    }

    void ShaderStore::addShader(OpenGLShaderProgram *shader)
    {
        m_ShaderList.append(shader);

        Q_ASSERT_X(!m_ShaderNameTable.contains(shader->objectName()),
                   Q_FUNC_INFO,
                   "Shader already exists!");

        // The ID of the shader is 1-based - 0 is null.
        m_ShaderNameTable.insert(shader->objectName(), m_ShaderList.count());
        shader->m_iShaderStoreId = m_ShaderList.count();
    }

    void ShaderStore::constructShaders()
    {
        GL_CURRENT_F;
        Q_ASSERT_X(f, Q_FUNC_INFO, "Must have a current OpenGL context!");

        foreach ( OpenGLShaderProgram* shader, m_ShaderList )
        {
            shader->construct();

            shader->bind();
            GLuint blockIndex = f->glGetUniformBlockIndex(shader->programId(), ShaderDefs::LOCAL_UNIFORM_BLOCK_NAME);
            if ( blockIndex != GL_INVALID_INDEX )
            {
                f->glUniformBlockBinding(shader->programId(), blockIndex, ShaderDefs::LocalUniformBlockBindingPoint);
            }
            shader->release();
        }
    }

    OpenGLShaderProgram* ShaderStore::shader(const QString &name) const
    {
        quint32 index = m_ShaderNameTable.value(name, 0);
        if ( index < 1 )
            return NULL;

        return m_ShaderList.at(index-1);
    }

    OpenGLShaderProgram* ShaderStore::shader(quint16 id) const
    {
        return m_ShaderList.at(id-1);
    }

    quint16 ShaderStore::shaderId(const QString &name) const
    {
        return m_ShaderNameTable.value(name, 0);
    }
}
