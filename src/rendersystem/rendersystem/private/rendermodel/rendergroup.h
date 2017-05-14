#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include <QSharedPointer>
#include <QMap>

#include "rendergroupkey.h"
#include "geometrydata.h"
#include "geometrydatakey.h"

#include "rendersystem/interface-classes/rendermodel-defs/publicrendermodeldefs.h"
#include "rendersystem/interface-classes/geometry/geometrysection.h"

class RenderGroup
{
public:
    RenderGroup(const RenderGroupKey& key);

    void setGeometry(const QSharedPointer<RenderSystem::GeometrySection>& section);
    void removeGeometry(RenderSystem::PublicRenderModelDefs::ObjectId objectId);

private:
    typedef QSharedPointer<GeometryData> GeometryDataPointer;

    GeometryDataPointer sectionToGeometryData(const QSharedPointer<RenderSystem::GeometrySection>& section);
    void generateIndices(const GeometryDataPointer& data);

    const RenderGroupKey m_Key;

    QMap<GeometryDataKey, GeometryDataPointer> m_Data;
};

#endif // RENDERGROUP_H
