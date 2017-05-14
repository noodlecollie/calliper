#include "geometrydatakey.h"

GeometryDataKey::GeometryDataKey(RenderSystem::PublicRenderModelDefs::ObjectId object,
                                 RenderSystem::PublicRenderModelDefs::SectionId section)
    : objectId(object),
      sectionId(section)
{
}

bool GeometryDataKey::operator <(const GeometryDataKey& other) const
{
    if ( objectId != other.objectId )
    {
        return objectId < other.objectId;
    }

    return sectionId < other.sectionId;
}
