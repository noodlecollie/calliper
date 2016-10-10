#include "matrixbatchitemkey.h"
#include <QByteArray>
#include <QDataStream>

namespace NS_RENDERER
{
    uint qHash(const MatrixBatchItemKey &key, uint seed)
    {
        QByteArray arr;

        {
            QDataStream stream(&arr, QIODevice::WriteOnly);
            stream << key.objectId() << key.sectionId();
        }

        return qHash(arr, seed);
    }

    MatrixBatchItemKey::MatrixBatchItemKey(quint32 objectId, quint8 sectionId)
        : m_iObjectId(objectId), m_iSectionId(sectionId)
    {

    }

    quint32 MatrixBatchItemKey::objectId() const
    {
        return m_iObjectId;
    }

    quint8 MatrixBatchItemKey::sectionId() const
    {
        return m_iSectionId;
    }
}
