#include "bufferdatacontainer.h"

#include <QtMath>

BufferDataContainer::BufferDataContainer(int maxBatches, int maxItemsPerBatch)
    : m_nNumBatches(maxBatches),
      m_nNumItemsPerBatch(maxItemsPerBatch),
      m_Items()
{

}

int BufferDataContainer::numBatches() const
{
    return m_nNumBatches;
}

int BufferDataContainer::numItemsPerBatch() const
{
    return m_nNumItemsPerBatch;
}

int BufferDataContainer::maxBatchItems() const
{
    return numBatches() * numItemsPerBatch();
}

int BufferDataContainer::batchItemCount() const
{
    return m_Items.count();
}

int BufferDataContainer::batchCount() const
{
    float batchItems = static_cast<float>(batchItemCount());
    float maxItemsInBatch = static_cast<float>(maxBatchItems());
    return static_cast<int>(ceil(batchItems/maxItemsInBatch));
}

bool BufferDataContainer::isFull() const
{
    return batchItemCount() >= maxBatchItems();
}

bool BufferDataContainer::isEmpty() const
{
    return m_Items.isEmpty();
}

bool BufferDataContainer::addGeometryData(const ObjectSectionGeometryDataPointer& data)
{
    if ( isFull() )
    {
        return false;
    }

    m_Items.append(data);
    return true;
}

void BufferDataContainer::clearItems()
{
    m_Items.clear();
}

const BufferDataContainer::ObjectSectionGeometryDataPointer& BufferDataContainer::geometryData(int batchItem) const
{
    return m_Items.at(batchItem);
}
