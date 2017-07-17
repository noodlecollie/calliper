#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include <QSharedPointer>
#include <QMap>
#include <QVector>
#include <QHash>

#include "rendermodelcontext.h"

#include "rendersystem/private/rendermodel/partition/geometrydatakey.h"
#include "rendersystem/private/rendermodel/partition/renderpartition.h"

#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"
#include "rendersystem/interface-classes/geometry/geometrysection.h"
#include "rendersystem/interface-classes/texture/namedopengltexture.h"

class RenderGroup
{
public:
    RenderGroup(const RenderModelContext& context,
                RenderSystem::MaterialDefs::MaterialId materialId);

    void setGeometry(const QSharedPointer<RenderSystem::GeometrySection>& section);
    void removeGeometry(RenderSystem::RenderModelDefs::ObjectId objectId);

    void draw();

private:
    void bindTexture();
    void releaseTexture();

    typedef QSharedPointer<RenderPartition> RenderPartitionPointer;

    const RenderModelContext& m_Context;
    const RenderSystem::MaterialDefs::MaterialId m_nMaterialId;

    QVector<RenderPartitionPointer> m_Partitions;
    QHash<GeometryDataKey, RenderPartitionPointer> m_SectionToPartition;
    QSharedPointer<RenderSystem::NamedOpenGLTexture> m_pCurrentTexture;
};

#endif // RENDERGROUP_H
