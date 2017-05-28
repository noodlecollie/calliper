#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include <QMap>
#include <QMultiHash>

#include "rendergroup.h"
#include "rendermodelcontext.h"

#include "rendersystem/interface-classes/shader/publicshaderdefs.h"
#include "rendersystem/interface-classes/store/publicstoredefs.h"
#include "rendersystem/interface-classes/geometry/geometrybuilder.h"
#include "rendersystem/interface-classes/rendermodel/publicrendermodeldefs.h"

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

    RenderModelContext m_Context;
    QMap<RenderSystem::PublicStoreDefs::MaterialId, RenderGroupPointer> m_RenderGroups;
    QMultiHash<RenderSystem::PublicRenderModelDefs::ObjectId, RenderGroupPointer> m_ObjectIdToRenderGroup;
};

#endif // RENDERMODEL_H
