#include "rendermodel.h"

#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"

RenderModel::RenderModel()
{

}

void RenderModel::setGeometry(const RenderSystem::GeometryBuilder &geometry)
{
    removeGeometry(geometry.objectId());

    for ( int i = 0; i < geometry.sectionCount(); ++i )
    {
        setGeometry(geometry.section(i));
    }
}

void RenderModel::setGeometry(const QSharedPointer<RenderSystem::GeometrySection> &section)
{
    if ( section->isEmpty() )
    {
        return;
    }

    RenderGroupKey key(section->materialId());
    RenderGroupPointer renderGroup = m_RenderGroups.value(key, RenderGroupPointer());

    if ( !renderGroup )
    {
        renderGroup = RenderGroupPointer::create(key);
        m_RenderGroups.insert(key, renderGroup);
    }

    renderGroup->setGeometry(section);
}

void RenderModel::removeGeometry(RenderSystem::PublicRenderModelDefs::ObjectId objectId)
{
    QList<RenderGroupPointer> renderGroups = m_ObjectIdToRenderGroup.values(objectId);

    for ( const RenderGroupPointer& group : renderGroups )
    {
        group->removeGeometry(objectId);
    }

    m_ObjectIdToRenderGroup.remove(objectId);
}
