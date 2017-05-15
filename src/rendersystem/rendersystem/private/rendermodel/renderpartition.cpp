#include "renderpartition.h"

RenderPartition::RenderPartition(int maxItems)
    : m_nMaxItems(maxItems)
{

}

int RenderPartition::maxItems() const
{
    return m_nMaxItems;
}

int RenderPartition::itemCount() const
{
    return m_Data.count();
}

bool RenderPartition::isFull() const
{
    return m_Data.count() >= m_nMaxItems;
}

void RenderPartition::setGeometry(const QSharedPointer<RenderSystem::GeometrySection> &section)
{
    if ( isFull() )
    {
        return;
    }

    m_Data.insert(GeometryDataKey(section->objectId(), section->sectionId()), sectionToGeometryData(section));
}

void RenderPartition::removeGeometry(RenderSystem::PublicRenderModelDefs::ObjectId objectId)
{
    QMap<GeometryDataKey, GeometryDataPointer>::iterator it = m_Data.begin();
    while ( it != m_Data.end() )
    {
        if ( it.key().objectId == objectId )
        {
            it = m_Data.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

RenderPartition::GeometryDataPointer RenderPartition::sectionToGeometryData(const QSharedPointer<RenderSystem::GeometrySection> &section)
{
    GeometryDataPointer data = GeometryDataPointer::create(section->objectId(), section->sectionId());

    data->setModelToWorldMatrix(section->modelToWorldMatrix());
    data->positionsRef() = section->attributeVector(RenderSystem::GeometrySection::Position);
    data->normalsRef() = section->attributeVector(RenderSystem::GeometrySection::Normal);
    data->colorsRef() = section->attributeVector(RenderSystem::GeometrySection::Color);
    data->textureCoordinatesRef() = section->attributeVector(RenderSystem::GeometrySection::TextureCoordinate);

    generateIndices(data);
    return data;
}

void RenderPartition::generateIndices(const GeometryDataPointer &data)
{
    const quint32 positionCount = data->positions().count();
    if ( positionCount < 3 )
    {
        return;
    }

    QVector<quint32>& indices = data->indicesRef();

    // (positionCount - 2) triangles, with 3 indices per triangle.
    indices.resize((positionCount - 2) * 3);

    int indexOffset = 0;
    for ( quint32 index = 2; index < positionCount; ++index )
    {
        indices[indexOffset] = 0;
        indices[indexOffset + 1] = index - 1;
        indices[indexOffset + 2] = index;

        indexOffset += 3;
    }
}
