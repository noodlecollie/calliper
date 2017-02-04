#include "rendermodelkey.h"
#include <QByteArray>
#include <QDataStream>

uint qHash(const Renderer::RenderModelKey &key, uint seed)
{
    QByteArray arr;

    {
        QDataStream stream(&arr, QIODevice::WriteOnly);
        stream << qHash(key.matrixBatchItemKey(), seed)
               << qHash(key.matrixBatchKey(), seed)
               << qHash(key.batchGroupKey(), seed)
               << qHash(key.passKey(), seed);
    }

    return qHash(arr, seed);
}

namespace Renderer
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
