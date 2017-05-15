#include "rendergroup.h"

namespace
{
    // Tweak as necessary, for performance.
    const int ITEMS_PER_PARTITION = 8 * 8;  // Batches * items per batch
}

RenderGroup::RenderGroup(const RenderGroupKey &key)
    : m_Key(key)
{

}

void RenderGroup::setGeometry(const QSharedPointer<RenderSystem::GeometrySection> &section)
{
    static_assert(ITEMS_PER_PARTITION > 0, "There must be at least 1 item per partition!");

    for ( int i = 0; i < m_Partitions.count(); ++i )
    {
        RenderPartitionPointer partition = m_Partitions.at(i);
        if ( partition->isFull() )
        {
            continue;
        }

        partition->setGeometry(section);
        m_SectionToPartition.insert(GeometryDataKey(section->objectId(), section->sectionId()), partition);
        return;
    }

    RenderPartitionPointer partition = RenderPartitionPointer::create(ITEMS_PER_PARTITION);
    partition->setGeometry(section);
    m_Partitions.append(partition);
    m_SectionToPartition.insert(GeometryDataKey(section->objectId(), section->sectionId()), partition);
}

void RenderGroup::removeGeometry(RenderSystem::PublicRenderModelDefs::ObjectId objectId)
{
    QSet<RenderPartition*> processedPartitions;

    QHash<GeometryDataKey, RenderPartitionPointer>::iterator it = m_SectionToPartition.begin();
    while ( it != m_SectionToPartition.end() )
    {
        // We can store raw pointers because the object will not be killed off,
        // as it still lives inside the partitions vector.
        RenderPartition* partition = it.value().data();

        if ( it.key().objectId != objectId || processedPartitions.contains(partition) )
        {
            ++it;
            continue;
        }

        partition->removeGeometry(objectId);
        it = m_SectionToPartition.erase(it);
        processedPartitions.insert(partition);
    }
}