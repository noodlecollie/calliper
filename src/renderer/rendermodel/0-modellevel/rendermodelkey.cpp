#include "rendermodelkey.h"

namespace NS_RENDERER
{
    RenderModelKey::RenderModelKey(const RenderModelPassKey& passKey, const RenderModelBatchGroupKey& batchGroupKey,
                                   const MatrixBatchKey& matrixBatchKey, const MatrixBatchItemKey& matrixBatchItemKey)
        : m_PassKey(passKey), m_BatchGroupKey(batchGroupKey), m_MatrixBatchKey(matrixBatchKey), m_MatrixBatchItemKey(matrixBatchItemKey)
    {

    }

    const RenderModelPassKey& RenderModelKey::passKey() const
    {
        return m_PassKey;
    }

    const RenderModelBatchGroupKey& RenderModelKey::batchGroupKey() const
    {
        return m_BatchGroupKey;
    }

    const MatrixBatchKey& RenderModelKey::matrixBatchKey() const
    {
        return m_MatrixBatchKey;
    }

    const MatrixBatchItemKey RenderModelKey::matrixBatchItemKey() const
    {
        return m_MatrixBatchItemKey;
    }
}
