#include "rendermodelpass.h"
#include "opengl/openglshaderprogram.h"
#include "stores/shaderstore.h"

namespace NS_RENDERER
{
    RenderModelPass::RenderModelPass()
    {

    }

    RenderModelPass::~RenderModelPass()
    {
        clear();
    }

    void RenderModelPass::addItem(const RenderModelBatchKey &key, const RenderModelBatchParams &params, QOpenGLBuffer::UsagePattern usagePattern)
    {
        Q_ASSERT_X(ShaderStore::getShaderStore(),
                 Q_FUNC_INFO,
                 "Shader store not intialised!");

        // TODO: Implement a fallback shader.
        OpenGLShaderProgram* program = ShaderStore::getShaderStore()->shader(key.shaderStoreId());
        Q_ASSERT_X(program, Q_FUNC_INFO, "Invalid shader specified in key!");

        RenderModelBatch* batch = NULL;
        if ( !m_Table.contains(key) || (batch = m_Table.find(key).value())->isFull() )
        {
            batch = new RenderModelBatch(usagePattern, program);
            m_Table.insert(key, batch);
            m_KeyOrdering.insert(key, 0);
        }

        // This should never happen, but just to be safe:
        Q_ASSERT_X(batch, Q_FUNC_INFO, "No render batch pointer acquired!");

        batch->create();
        batch->addItem(params);
    }

    void RenderModelPass::clear()
    {
        qDeleteAll(m_Table.values());
        m_Table.clear();
        m_KeyOrdering.clear();
    }

    void RenderModelPass::debugDraw(QOpenGLTexture *texture)
    {
        foreach ( const RenderModelBatchKey &key, m_KeyOrdering.keys() )
        {
            RenderModelBatch* batch = m_Table.value(key);

            OpenGLShaderProgram* program = ShaderStore::getShaderStore()->shader(key.shaderStoreId());
            program->bind();
            program->enableAttributeArrays();

            batch->bindDraw();
            texture->bind(0);
            batch->setAttributePointers(program);
            batch->draw();
            texture->release();
            batch->releaseDraw();

            program->disableAttributeArrays();
        }
    }

    void RenderModelPass::debugUploadAll()
    {
        foreach ( RenderModelBatch* batch, m_Table.values() )
        {
            batch->upload();
        }
    }
}
