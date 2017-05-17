#include "geometrydatakey.h"
#include <QByteArray>

uint qHash(const GeometryDataKey& key, uint seed)
{
    // It's less likely that the upper byte of the object ID will be used,
    // so do an XOR with that.
    return qHash((static_cast<quint32>(key.sectionId) << 24) ^ key.objectId, seed);
}

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

bool GeometryDataKey::operator ==(const GeometryDataKey& other) const
{
    return objectId == other.objectId && sectionId == other.sectionId;
}
