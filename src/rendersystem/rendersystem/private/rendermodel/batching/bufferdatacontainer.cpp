#include "bufferdatacontainer.h"

#include <QtMath>

BufferDataContainer::BufferDataContainer(int maxBatches, int maxItemsPerBatch)
    : m_nMaxBatches(maxBatches),
      m_nMaxItemsPerBatch(maxItemsPerBatch),
      m_Items()
{

}

int BufferDataContainer::maxBatches() const
{
    return m_nMaxBatches;
}

int BufferDataContainer::maxItemsPerBatch() const
{
    return m_nMaxItemsPerBatch;
}

int BufferDataContainer::maxSupportedBatchItems() const
{
    return maxBatches() * maxItemsPerBatch();
}

int BufferDataContainer::totalBatchItems() const
{
    return m_Items.count();
}

int BufferDataContainer::totalBatches() const
{
    float batchItems = static_cast<float>(totalBatchItems());
    float maxItemsInBatch = static_cast<float>(maxItemsPerBatch());
    return static_cast<int>(ceil(batchItems/maxItemsInBatch));
}

bool BufferDataContainer::isFull() const
{
    return totalBatchItems() >= maxSupportedBatchItems();
}

bool BufferDataContainer::isEmpty() const
{
    return m_Items.isEmpty();
}

bool BufferDataContainer::addItem(const ObjectDataPointer& data)
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

const BufferDataContainer::ObjectDataPointer& BufferDataContainer::objectData(int batchItem) const
{
    return m_Items.at(batchItem);
}
