#ifndef GEOMETRYDATAKEY_H
#define GEOMETRYDATAKEY_H

#include <QHash>

#include "rendersystem/interface-classes/definitions/publicrendermodeldefs.h"

struct GeometryDataKey
{
    RenderSystem::PublicRenderModelDefs::ObjectId objectId;
    RenderSystem::PublicRenderModelDefs::SectionId sectionId;

    GeometryDataKey(RenderSystem::PublicRenderModelDefs::ObjectId object,
                    RenderSystem::PublicRenderModelDefs::SectionId section);
    bool operator <(const GeometryDataKey& other) const;
    bool operator ==(const GeometryDataKey& other) const;
};

uint qHash(const GeometryDataKey& key, uint seed = 0);

#endif // GEOMETRYDATAKEY_H
