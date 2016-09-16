#include "matrixbatchitemkey.h"

uint qHash(const NS_RENDERER::MatrixBatchItemKey &key, uint seed)
{
    return qHash(key.objectId(), seed);
}

namespace NS_RENDERER
{
    MatrixBatchItemKey::MatrixBatchItemKey(quint32 objectId)
        : m_iObjectId(objectId)
    {

    }

    quint32 MatrixBatchItemKey::objectId() const
    {
        return m_iObjectId;
    }
}
