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

    bool addItem(const ObjectDataPointer& data);
    void clearItems();

    const ObjectDataPointer& objectData(int batchItem) const;

private:
    const int m_nMaxBatches;
    const int m_nMaxItemsPerBatch;

    QVector<ObjectDataPointer> m_Items;
};

#endif // BUFFERDATACONTAINER_H
