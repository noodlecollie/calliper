#include "rendergroup.h"

namespace
{
    // Tweak as necessary, for performance.
    const int ITEMS_PER_BATCH = 8;
    const int BATCHES_PER_PARTITION = 8;
    const int ITEMS_PER_PARTITION = ITEMS_PER_BATCH * BATCHES_PER_PARTITION;
}

RenderGroup::RenderGroup(const RenderModelContext &context,
                         RenderSystem::PublicStoreDefs::MaterialId materialId)
    : m_Context(context),
      m_nMaterialId(materialId),
      m_Partitions(),
      m_SectionToPartition()
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

    RenderPartitionPointer partition = RenderPartitionPointer::create(m_Context, m_nMaterialId, ITEMS_PER_PARTITION);
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
