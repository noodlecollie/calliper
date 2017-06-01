#ifndef GEOMETRYDATACONTAINER_H
#define GEOMETRYDATACONTAINER_H

#include <QMap>
#include <QSharedPointer>

#include "geometrydata.h"
#include "geometrydatakey.h"

#include "rendersystem/interface-classes/geometry/geometrysection.h"

class GeometryDataContainer
{
public:
    typedef QSharedPointer<GeometryData> GeometryDataPointer;
    typedef QMap<GeometryDataKey, GeometryDataPointer>::const_iterator ConstIterator;
    typedef QMap<GeometryDataKey, GeometryDataPointer>::iterator Iterator;

    GeometryDataContainer();

    void insert(const QSharedPointer<RenderSystem::GeometrySection>& section);
    void insert(const GeometryDataKey& key, const GeometryDataPointer& data);
    void remove(RenderSystem::PublicRenderModelDefs::ObjectId objectId);
    void clear();

    int count() const;
    ConstIterator constBegin() const;
    ConstIterator constEnd() const;
    Iterator begin();
    Iterator end();

private:
    static GeometryDataPointer sectionToGeometryData(const QSharedPointer<RenderSystem::GeometrySection>& section);

    QMap<GeometryDataKey, GeometryDataPointer> m_Data;
};

#endif // GEOMETRYDATACONTAINER_H
