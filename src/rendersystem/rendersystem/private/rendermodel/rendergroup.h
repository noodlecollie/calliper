#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include <QSharedPointer>

#include "rendersystem/interface-classes/rendermodel-defs/publicrendermodeldefs.h"
#include "rendersystem/interface-classes/geometry/geometrysection.h"
#include "rendergroupkey.h"

class RenderGroup
{
public:
    RenderGroup(const RenderGroupKey& key);

    void setGeometry(const QSharedPointer<RenderSystem::GeometrySection>& section);
    void removeGeometry(RenderSystem::PublicRenderModelDefs::ObjectId objectId);

private:
    const RenderGroupKey m_Key;
};

#endif // RENDERGROUP_H
