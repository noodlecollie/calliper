#include "batchgenerator.h"

BatchGenerator::BatchGenerator(const GeometryDataContainer &data)
    : m_Data(data),
      m_Batches(),
      m_nMaxBatchedItems(0),
      m_itLastProcessedItem(m_Data.constEnd())
{

}

const BatchGenerator::GeometryDataVector& BatchGenerator::batch(int index) const
{
    return m_Batches.at(index);
}

int BatchGenerator::batchCount() const
{
    return m_Batches.count();
}

void BatchGenerator::buildBatches(int maxItemsPerBatch)
{
    m_Batches.clear();

    m_nMaxBatchedItems = maxItemsPerBatch;
    if ( m_nMaxBatchedItems < 1 )
    {
        return;
    }

    m_itLastProcessedItem = m_Data.constEnd();

    for ( GeometryDataContainer::ConstIterator itGeometry = m_Data.constBegin();
          itGeometry != m_Data.constEnd();
          m_itLastProcessedItem = itGeometry++ )
    {
        GeometryDataVector& batch = nextCompatibleBatch(itGeometry.key());
        batch.append(itGeometry.value());
    }
}

BatchGenerator::GeometryDataVector& BatchGenerator::nextCompatibleBatch(const GeometryDataKey &key)
{
    if ( m_Batches.isEmpty() ||
         m_Batches.last().count() >= m_nMaxBatchedItems ||
         (m_itLastProcessedItem != m_Data.constEnd() && !m_itLastProcessedItem.key().batchableWith(key)) )
    {
        m_Batches.append(GeometryDataVector());
    }

    return m_Batches.last();
}
