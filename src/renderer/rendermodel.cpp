#include "rendermodel.h"
#include "openglshaderprogram.h"
#include "shaderstore.h"

namespace NS_RENDERER
{
    RenderModel::RenderModel()
    {

    }

    RenderModel::~RenderModel()
    {
        clear();
    }

    void RenderModel::addItem(const RenderModelBatchKey &key, const RenderModelBatchParams &params, QOpenGLBuffer::UsagePattern usagePattern)
    {
        Q_ASSERT_X(ShaderStore::getShaderStore(),
                 Q_FUNC_INFO,
                 "Shader store not intialised!");

        // TODO: Implement a fallback shader.
        OpenGLShaderProgram* program = ShaderStore::getShaderStore()->shader(key.shaderStoreId());

        RenderModelBatch* batch = NULL;
        if ( !m_Table.contains(key) || (batch = m_Table.find(key).value())->isFull() )
        {
            batch = new RenderModelBatch(usagePattern, program, program);
            m_Table.insert(key, batch);
        }

        // This should never happen, but just to be safe:
        Q_ASSERT_X(batch, Q_FUNC_INFO, "No render batch pointer acquired!");

        batch->create();
        batch->addItem(params);
    }

    void RenderModel::clear()
    {
        qDeleteAll(m_Table.values());
        m_Table.clear();
    }

    void RenderModel::debugDraw(QOpenGLTexture *texture)
    {
        foreach ( RenderModelBatch* batch, m_Table.values() )
        {
            batch->bindDraw();
            texture->bind(0);
            batch->setAttributePointers();
            batch->draw();
            texture->release();
            batch->releaseDraw();
        }
    }

    void RenderModel::debugUploadAll()
    {
        foreach ( RenderModelBatch* batch, m_Table.values() )
        {
            batch->upload();
        }
    }
}
