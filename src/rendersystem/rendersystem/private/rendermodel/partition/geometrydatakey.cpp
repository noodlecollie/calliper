#include "geometrydatakey.h"
#include <QByteArray>

namespace
{
    template<typename T>
    inline void coerceToBytes(char*& data, const T& value)
    {
        *reinterpret_cast<T*>(data) = value;
        data += sizeof(T);
    }
}

uint qHash(const GeometryDataKey& key, uint seed)
{
    using namespace RenderSystem::RenderModelDefs;

    const quint32 dataSize = sizeof(ObjectId) + sizeof(SectionId) + sizeof(GLenum);
    char arData[dataSize];
    char* data = arData;

    coerceToBytes(data, key.drawMode);
    coerceToBytes(data, key.objectId);
    coerceToBytes(data, key.sectionId);

    return qHashBits(arData, dataSize, seed);
}

GeometryDataKey::GeometryDataKey(GLenum mode,
                                 float width,
                                 RenderSystem::RenderModelDefs::ObjectId object,
                                 RenderSystem::RenderModelDefs::SectionId section)
    : drawMode(mode),
      lineWidth(width),
      objectId(object),
      sectionId(section)
{
}

bool GeometryDataKey::operator <(const GeometryDataKey& other) const
{
    if ( drawMode != other.drawMode )
    {
        return drawMode < other.drawMode;
    }

    if ( lineWidth != other.lineWidth )
    {
        return lineWidth < other.lineWidth;
    }

    if ( objectId != other.objectId )
    {
        return objectId < other.objectId;
    }

    return sectionId < other.sectionId;
}

bool GeometryDataKey::operator ==(const GeometryDataKey& other) const
{
    return drawMode == other.drawMode &&
            objectId == other.objectId &&
            sectionId == other.sectionId;
}
