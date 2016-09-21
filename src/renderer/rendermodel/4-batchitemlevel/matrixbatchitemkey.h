#ifndef MATRIXBATCHITEMKEY_H
#define MATRIXBATCHITEMKEY_H

#include "renderer_global.h"
#include <QHash>

namespace NS_RENDERER
{
    class MatrixBatchItemKey
    {
    public:
        explicit MatrixBatchItemKey(quint32 objectId);

        quint32 objectId() const;

        inline bool operator <(const MatrixBatchItemKey &other) const
        {
            return m_iObjectId < other.m_iObjectId;
        }

        inline bool operator ==(const MatrixBatchItemKey &other) const
        {
            return m_iObjectId == other.m_iObjectId;
        }

        inline bool operator !=(const MatrixBatchItemKey &other) const
        {
            return !(*this == other);
        }

    private:
        const quint32   m_iObjectId;
    };

    RENDERERSHARED_EXPORT uint qHash(const MatrixBatchItemKey &key, uint seed = 0);
}

#endif // MATRIXBATCHITEMKEY_H
