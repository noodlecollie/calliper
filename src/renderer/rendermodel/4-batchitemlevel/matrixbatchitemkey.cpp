#include "matrixbatchitemkey.h"

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
