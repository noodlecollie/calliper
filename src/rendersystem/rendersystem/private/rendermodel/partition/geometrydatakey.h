#ifndef GEOMETRYDATAKEY_H
#define GEOMETRYDATAKEY_H

#include <QHash>

#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"

struct GeometryDataKey
{
    RenderSystem::RenderModelDefs::ObjectId objectId;
    RenderSystem::RenderModelDefs::SectionId sectionId;

    GeometryDataKey(RenderSystem::RenderModelDefs::ObjectId object,
                    RenderSystem::RenderModelDefs::SectionId section);
    bool operator <(const GeometryDataKey& other) const;
    bool operator ==(const GeometryDataKey& other) const;
};

uint qHash(const GeometryDataKey& key, uint seed = 0);

#endif // GEOMETRYDATAKEY_H
