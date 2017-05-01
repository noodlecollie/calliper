#ifndef BUFFERDATACONTAINER_H
#define BUFFERDATACONTAINER_H

#include <QSharedPointer>
#include <QVector>

#include "rendersystem/private/rendermodel/raw-data/objectsectiongeometrydata.h"

class BufferDataContainer
{
public:
    typedef QSharedPointer<ObjectSectionGeometryData> ObjectDataPointer;

    BufferDataContainer(int maxBatches, int maxItemsPerBatch);

    int maxBatches() const;
    int maxItemsPerBatch() const;
    int maxSupportedBatchItems() const;

    // Total number of batches that are at least partially full.
    int totalBatches() const;

    int totalBatchItems() const;

    bool isFull() const;
    bool isEmpty() const;

    bool addItem(const QMatrix4x4 modelToWorldMatrix, const ObjectDataPointer& data);
    void clearItems();

private:
    struct ItemData
    {
        QMatrix4x4 m_matModelToWorld;
        ObjectDataPointer m_Data;

        ItemData(const QMatrix4x4& mat, const ObjectDataPointer& data)
            : m_matModelToWorld(mat),
              m_Data(data)
        {
        }
    };

    const int m_nMaxBatches;
    const int m_nMaxItemsPerBatch;

    QVector<ItemData> m_Items;
};

#endif // BUFFERDATACONTAINER_H
