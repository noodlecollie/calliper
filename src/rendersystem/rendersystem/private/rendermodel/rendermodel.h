#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include <QHash>
#include <QMultiHash>
#include <QMatrix4x4>
#include <QSharedPointer>
#include <QOpenGLFramebufferObject>

#include "rendergroup.h"
#include "rendermodelcontext.h"

#include "rendersystem/interface-classes/definitions/materialdefs.h"
#include "rendersystem/interface-classes/geometry/geometrybuilder.h"
#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"
#include "rendersystem/interface-classes/definitions/framebufferdefs.h"
#include "rendersystem/private/shaders/common/globalshaderuniforms.h"

class RenderModel
{
public:
    RenderModel();
    ~RenderModel();

    // Replaces any geometry for the given object.
    void setGeometry(const RenderSystem::GeometryBuilder& geometry);
    void removeGeometry(RenderSystem::RenderModelDefs::ObjectId objectId);
    void clear();

    void draw(RenderSystem::FrameBufferDefs::FrameBufferId frameBufferId,
              RenderSystem::ShaderDefs::RenderMode renderMode,
              const QMatrix4x4& worldToCameraMatrix,
              const QMatrix4x4& projectionMatrix);

private:
    typedef QSharedPointer<RenderGroup> RenderGroupPointer;
    typedef QHash<RenderSystem::MaterialDefs::MaterialId, RenderGroupPointer> RenderGroupHash;

    void setGeometry(const QSharedPointer<RenderSystem::GeometrySection>& section);
    QSharedPointer<QOpenGLFramebufferObject> frameBuffer(RenderSystem::FrameBufferDefs::FrameBufferId id) const;

    RenderModelContext m_Context;
    RenderGroupHash m_RenderGroups;
    QMultiHash<RenderSystem::RenderModelDefs::ObjectId, RenderGroupPointer> m_ObjectIdToRenderGroup;
    GlobalShaderUniforms m_GlobalShaderUniforms;
};

#endif // RENDERMODEL_H
