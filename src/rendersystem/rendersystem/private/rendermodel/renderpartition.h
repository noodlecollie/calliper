#ifndef RENDERPARTITION_H
#define RENDERPARTITION_H

#include <QOpenGLBuffer>
#include <QSharedPointer>

#include "geometrydatakey.h"
#include "geometrydata.h"

#include "rendersystem/interface-classes/geometry/geometrysection.h"

class RenderPartition
{
public:
    RenderPartition(int maxItems);

    int maxItems() const;
    int itemCount() const;
    bool isFull() const;
    int freeSpaces() const;

    void setGeometry(const QSharedPointer<RenderSystem::GeometrySection>& section);
    void removeGeometry(RenderSystem::PublicRenderModelDefs::ObjectId objectId);

private:
    typedef QSharedPointer<GeometryData> GeometryDataPointer;

    GeometryDataPointer sectionToGeometryData(const QSharedPointer<RenderSystem::GeometrySection>& section);
    void generateIndices(const GeometryDataPointer& data);

    const int m_nMaxItems;
    QMap<GeometryDataKey, GeometryDataPointer> m_Data;
};

#endif // RENDERPARTITION_H
