#ifndef RENDERMODELBATCHITEMKEY_H
#define RENDERMODELBATCHITEMKEY_H

#include "renderer_global.h"
#include <QMatrix4x4>
#include <QHash>
#include "general/hashfunctions.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT RenderModelBatchItemKey
    {
    public:
        RenderModelBatchItemKey(const QMatrix4x4 &mat, quint64 id);

        const QMatrix4x4& matrix() const;
        quint64 objectUniqueId() const;

        inline bool operator ==(const RenderModelBatchItemKey &other) const
        {
            return m_matMatrix == other.m_matMatrix &&
                    m_iObjectUniqueId == other.m_iObjectUniqueId;
        }

        inline bool operator <(const RenderModelBatchItemKey &other) const
        {
            if ( m_matMatrix != other.m_matMatrix )
                return qHash(m_matMatrix, 0) < qHash(other.m_matMatrix, 0);

            return m_iObjectUniqueId < other.m_iObjectUniqueId;
        }

    private:
        QMatrix4x4  m_matMatrix;
        quint64     m_iObjectUniqueId;
    };

    uint RENDERERSHARED_EXPORT qHash(const RenderModelBatchItemKey &key, uint seed);
}

#endif // RENDERMODELBATCHITEMKEY_H
