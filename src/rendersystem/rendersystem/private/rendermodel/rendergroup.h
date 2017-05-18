#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include <QSharedPointer>
#include <QMap>
#include <QVector>
#include <QHash>

#include "rendergroupkey.h"
#include "rendersystem/private/rendermodel/partition/geometrydatakey.h"
#include "rendersystem/private/rendermodel/partition/renderpartition.h"

#include "rendersystem/interface-classes/rendermodel-defs/publicrendermodeldefs.h"
#include "rendersystem/interface-classes/geometry/geometrysection.h"

class RenderGroup
{
public:
    RenderGroup(const RenderGroupKey& key);

    void setGeometry(const QSharedPointer<RenderSystem::GeometrySection>& section);
    void removeGeometry(RenderSystem::PublicRenderModelDefs::ObjectId objectId);

private:
    typedef QSharedPointer<RenderPartition> RenderPartitionPointer;

    const RenderGroupKey m_Key;
    QVector<RenderPartitionPointer> m_Partitions;
    QHash<GeometryDataKey, RenderPartitionPointer> m_SectionToPartition;
};

#endif // RENDERGROUP_H
