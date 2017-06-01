#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include <QMap>
#include <QMultiHash>

#include "rendergroup.h"
#include "rendermodelcontext.h"

#include "rendersystem/interface-classes/definitions/materialdefs.h"
#include "rendersystem/interface-classes/geometry/geometrybuilder.h"
#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"

class RenderModel
{
public:
    RenderModel();

    // Replaces any geometry for the given object.
    void setGeometry(const RenderSystem::GeometryBuilder& geometry);
    void removeGeometry(RenderSystem::RenderModelDefs::ObjectId objectId);

private:
    typedef QSharedPointer<RenderGroup> RenderGroupPointer;

    void setGeometry(const QSharedPointer<RenderSystem::GeometrySection>& section);

    RenderModelContext m_Context;
    QMap<RenderSystem::MaterialDefs::MaterialId, RenderGroupPointer> m_RenderGroups;
    QMultiHash<RenderSystem::RenderModelDefs::ObjectId, RenderGroupPointer> m_ObjectIdToRenderGroup;
};

#endif // RENDERMODEL_H
