#include "rendergroup.h"

RenderGroup::RenderGroup(const RenderGroupKey &key)
    : m_Key(key)
{

}

void RenderGroup::setGeometry(const QSharedPointer<RenderSystem::GeometrySection> &section)
{
    static_assert(false, "Complete me");
}

void RenderGroup::removeGeometry(RenderSystem::PublicRenderModelDefs::ObjectId objectId)
{
    static_assert(false, "Complete me");
}
