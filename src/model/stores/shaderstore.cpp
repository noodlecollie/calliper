#include "shaderstore.h"

namespace NS_MODEL
{
    ShaderStore::ShaderStore()
        : m_iNextShaderId(1)
    {

    }

    ShaderStore::~ShaderStore()
    {
        using namespace NS_RENDERER;

        foreach ( OpenGLShaderProgram* shaderProgram, m_ShaderTable.values() )
        {
            delete shaderProgram;
        }

        m_ShaderTable.clear();
    }

    quint16 ShaderStore::acquireNextShaderId()
    {
        Q_ASSERT_X(m_iNextShaderId + 1 != 0, Q_FUNC_INFO, "Shader ID counter overflow!");
        return m_iNextShaderId++;
    }

    NS_RENDERER::OpenGLShaderProgram* ShaderStore::getShaderProgram(quint16 shaderId) const
    {
        return m_ShaderTable.value(shaderId, nullptr);
    }

    NS_RENDERER::OpenGLShaderProgram* ShaderStore::operator ()(quint16 shaderId) const
    {
        return getShaderProgram(shaderId);
    }

    quint16 ShaderStore::getShaderId(const QString shaderName) const
    {
        using namespace NS_RENDERER;

        foreach ( OpenGLShaderProgram* shaderProgram, m_ShaderTable.values() )
        {
            if ( shaderProgram->objectName() == shaderName )
                return shaderProgram->shaderStoreId();
        }

        return 0;
    }

    NS_RENDERER::OpenGLShaderProgram* ShaderStore::getShaderProgram(const QString &shaderName) const
    {
        return getShaderProgram(getShaderId(shaderName));
    }

    void ShaderStore::addCategoryMapping(ShaderCategory category, quint16 shader)
    {
        m_ShaderCategoryTable.insert(category, shader);
    }

    void ShaderStore::removeCategoryMapping(ShaderCategory category)
    {
        m_ShaderCategoryTable.remove(category);
    }

    quint16 ShaderStore::shaderForCategory(ShaderCategory category) const
    {
        return m_ShaderCategoryTable.value(category, 0);
    }
}
