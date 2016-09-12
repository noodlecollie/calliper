#include "rendermodelbatchitemkey.h"
#include <QByteArray>
#include <QDataStream>

namespace NS_RENDERER
{
    uint qHash(const RenderModelBatchItemKey &key, uint seed)
    {
        QByteArray arr;

        {
            QDataStream stream(&arr, QIODevice::WriteOnly);
            stream.writeBytes(reinterpret_cast<const char*>(key.matrix().constData()), 16 * sizeof(float));
            stream << key.objectUniqueId();
        }

        return qHash(arr, seed);
    }

    RenderModelBatchItemKey::RenderModelBatchItemKey(const QMatrix4x4 &mat, quint64 id)
        : m_matMatrix(mat), m_iObjectUniqueId(id)
    {

    }

    const QMatrix4x4& RenderModelBatchItemKey::matrix() const
    {
        return m_matMatrix;
    }

    quint64 RenderModelBatchItemKey::objectUniqueId() const
    {
        return m_iObjectUniqueId;
    }
}
