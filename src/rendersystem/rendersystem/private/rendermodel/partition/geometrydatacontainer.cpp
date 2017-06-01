#include "geometrydatacontainer.h"

GeometryDataContainer::GeometryDataContainer()
{

}

int GeometryDataContainer::count() const
{
    return m_Data.count();
}

void GeometryDataContainer::insert(const QSharedPointer<RenderSystem::GeometrySection>& section)
{
    insert(GeometryDataKey(section->drawMode(),
                           section->lineWidth(),
                           section->objectId(),
                           section->sectionId()),
           sectionToGeometryData(section));
}

void GeometryDataContainer::insert(const GeometryDataKey &key, const GeometryDataPointer &data)
{
    m_Data.insert(key, data);
}

void GeometryDataContainer::remove(RenderSystem::RenderModelDefs::ObjectId objectId)
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

void GeometryDataContainer::clear()
{
    m_Data.clear();
}

GeometryDataContainer::ConstIterator GeometryDataContainer::constBegin() const
{
    return m_Data.constBegin();
}

GeometryDataContainer::ConstIterator GeometryDataContainer::constEnd() const
{
    return m_Data.constEnd();
}

GeometryDataContainer::Iterator GeometryDataContainer::begin()
{
    return m_Data.begin();
}

GeometryDataContainer::Iterator GeometryDataContainer::end()
{
    return m_Data.end();
}

GeometryDataContainer::GeometryDataPointer GeometryDataContainer::sectionToGeometryData(const QSharedPointer<RenderSystem::GeometrySection>& section)
{
    GeometryDataPointer data = GeometryDataPointer::create(section->objectId(), section->sectionId());

    data->setModelToWorldMatrix(section->modelToWorldMatrix());
    data->positionsRef() = section->attributeVector(RenderSystem::GeometrySection::Position);
    data->normalsRef() = section->attributeVector(RenderSystem::GeometrySection::Normal);
    data->colorsRef() = section->attributeVector(RenderSystem::GeometrySection::Color);
    data->textureCoordinatesRef() = section->attributeVector(RenderSystem::GeometrySection::TextureCoordinate);
    data->indicesRef() = section->indicesVector();

    data->setDrawMode(section->drawMode());
    data->setLineWidth(section->lineWidth());

    return data;
}
