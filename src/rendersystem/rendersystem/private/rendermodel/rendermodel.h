#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include <QHash>
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
    ~RenderModel();

    // Replaces any geometry for the given object.
    void setGeometry(const RenderSystem::GeometryBuilder& geometry);
    void removeGeometry(RenderSystem::RenderModelDefs::ObjectId objectId);
    void clear();

    void draw();

private:
    typedef QSharedPointer<RenderGroup> RenderGroupPointer;
    typedef QHash<RenderSystem::MaterialDefs::MaterialId, RenderGroupPointer> RenderGroupHash;

    void setGeometry(const QSharedPointer<RenderSystem::GeometrySection>& section);

    RenderModelContext m_Context;
    RenderGroupHash m_RenderGroups;
    QMultiHash<RenderSystem::RenderModelDefs::ObjectId, RenderGroupPointer> m_ObjectIdToRenderGroup;
};

#endif // RENDERMODEL_H
