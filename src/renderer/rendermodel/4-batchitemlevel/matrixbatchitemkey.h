#ifndef MATRIXBATCHITEMKEY_H
#define MATRIXBATCHITEMKEY_H

#include "renderer_global.h"
#include <QHash>

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT MatrixBatchItemKey
    {
    public:
        explicit MatrixBatchItemKey(quint32 objectId, quint8 sectionId);

        quint32 objectId() const;
        quint8 sectionId() const;

        inline bool operator <(const MatrixBatchItemKey &other) const
        {
            if ( m_iObjectId != m_iSectionId )
                return m_iObjectId < other.m_iObjectId;

            return m_iSectionId < other.m_iSectionId;
        }

        inline bool operator ==(const MatrixBatchItemKey &other) const
        {
            return m_iObjectId == other.m_iObjectId &&
                    m_iSectionId == other.m_iSectionId;
        }

        inline bool operator !=(const MatrixBatchItemKey &other) const
        {
            return !(*this == other);
        }

    private:
        quint32 m_iObjectId;
        quint8 m_iSectionId;
    };

    RENDERERSHARED_EXPORT uint qHash(const MatrixBatchItemKey &key, uint seed = 0);
}

#endif // MATRIXBATCHITEMKEY_H
