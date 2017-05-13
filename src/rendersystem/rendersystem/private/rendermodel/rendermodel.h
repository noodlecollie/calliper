#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include <QMap>
#include <QMultiHash>

#include "rendergroup.h"
#include "rendergroupkey.h"

#include "rendersystem/interface-classes/shader/publicshaderdefs.h"
#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"
#include "rendersystem/interface-classes/geometry/geometrybuilder.h"
#include "rendersystem/interface-classes/rendermodel-defs/publicrendermodeldefs.h"

class RenderModel
{
public:
    RenderModel();

    // Replaces any geometry for the given object.
    void setGeometry(const RenderSystem::GeometryBuilder& geometry);
    void removeGeometry(RenderSystem::PublicRenderModelDefs::ObjectId objectId);

private:
    typedef QSharedPointer<RenderGroup> RenderGroupPointer;

    void setGeometry(const QSharedPointer<RenderSystem::GeometrySection>& section);

    QMap<RenderGroupKey, RenderGroupPointer> m_RenderGroups;
    QMultiHash<RenderSystem::PublicRenderModelDefs::ObjectId, RenderGroupPointer> m_ObjectIdToRenderGroup;
};

#endif // RENDERMODEL_H
