#ifndef GEOMETRYDATAKEY_H
#define GEOMETRYDATAKEY_H

#include "rendersystem/interface-classes/rendermodel-defs/publicrendermodeldefs.h"

struct GeometryDataKey
{
    RenderSystem::PublicRenderModelDefs::ObjectId objectId;
    RenderSystem::PublicRenderModelDefs::SectionId sectionId;

    GeometryDataKey(RenderSystem::PublicRenderModelDefs::ObjectId object,
                    RenderSystem::PublicRenderModelDefs::SectionId section);
    bool operator <(const GeometryDataKey& other) const;
};

#endif // GEOMETRYDATAKEY_H
