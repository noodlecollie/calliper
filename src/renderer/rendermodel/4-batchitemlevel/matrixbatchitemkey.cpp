#include "matrixbatchitemkey.h"

namespace NS_RENDERER
{
    uint qHash(const MatrixBatchItemKey &key, uint seed)
    {
        return ::qHash(key.objectId(), seed);
    }

    MatrixBatchItemKey::MatrixBatchItemKey(quint32 objectId)
        : m_iObjectId(objectId)
    {

    }

    quint32 MatrixBatchItemKey::objectId() const
    {
        return m_iObjectId;
    }
}
