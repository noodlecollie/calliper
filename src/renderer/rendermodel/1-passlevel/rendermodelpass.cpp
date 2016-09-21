#include "rendermodelpass.h"
#include <QtDebug>

namespace NS_RENDERER
{
    RenderModelPass::RenderModelPass(IShaderRetrievalFunctor* shaderFunctor)
        : m_pShaderFunctor(shaderFunctor)
    {
        Q_ASSERT_X(m_pShaderFunctor, Q_FUNC_INFO, "Shader functor should not be null!");
    }

    RenderModelPass::~RenderModelPass()
    {
        clearBatchGroups();
    }

    RenderModelPass::RenderModelBatchGroupPointer RenderModelPass::createBatchGroup(const RenderModelBatchGroupKey &key, QOpenGLBuffer::UsagePattern usagePattern)
    {
        OpenGLShaderProgram* shaderProgram = (*m_pShaderFunctor)(key.shaderId());

        RenderModelBatchGroupPointer batchGroup = RenderModelBatchGroupPointer::create(usagePattern, shaderProgram);
        m_BatchGroups.insert(key, batchGroup);
        return batchGroup;
    }

    RenderModelPass::RenderModelBatchGroupPointer RenderModelPass::getBatchGroup(const RenderModelBatchGroupKey &key) const
    {
        return m_BatchGroups.value(key, RenderModelBatchGroupPointer());
    }

    void RenderModelPass::removeBatchGroup(const RenderModelBatchGroupKey &key)
    {
        m_BatchGroups.remove(key);
    }

    bool RenderModelPass::containsBatchGroup(const RenderModelBatchGroupKey &key) const
    {
        return m_BatchGroups.contains(key);
    }

    void RenderModelPass::clearBatchGroups()
    {
        m_BatchGroups.clear();
    }

    void RenderModelPass::printDebugInfo() const
    {
        qDebug() << "Batch groups:" << m_BatchGroups.count();
    }
}
