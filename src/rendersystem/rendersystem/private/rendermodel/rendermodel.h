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
#include "rendersystem/interface-classes/rendering/framedrawparams.h"
#include "rendersystem/private/shaders/common/globalshaderuniforms.h"

#include "calliperutil/opengl/openglcontextchecker.h"

#include "profiling/profilermodel/profilermodel.h"
#include "profiling/profilermodel/profileritemmodeladatper.h"

class RenderModel : private CalliperUtil::OpenGLContextChecker
{
public:
    RenderModel(const QString& name);
    ~RenderModel();

    QString name() const;
    void setName(const QString& newName);

    // Replaces any geometry for the given object.
    void setGeometry(const RenderSystem::GeometryBuilder& geometry);
    void removeGeometry(RenderSystem::RenderModelDefs::ObjectId objectId);
    void clear();

    void draw(RenderSystem::FrameBufferDefs::FrameBufferId frameBufferId,
              const RenderSystem::FrameDrawParams& drawParams);

    Profiling::ProfilerItemModelAdatper* profilingData();

private:
    typedef QSharedPointer<RenderGroup> RenderGroupPointer;
    typedef QHash<RenderSystem::MaterialDefs::MaterialId, RenderGroupPointer> RenderGroupHash;

    void setGeometry(const QSharedPointer<RenderSystem::GeometrySection>& section);
    QSharedPointer<QOpenGLFramebufferObject> frameBuffer(RenderSystem::FrameBufferDefs::FrameBufferId id) const;

    void drawPreFrame(const QSize& size, const RenderSystem::FrameDrawParams& drawParams);

    QString m_strName;
    RenderModelContext m_Context;
    RenderGroupHash m_RenderGroups;
    QMultiHash<RenderSystem::RenderModelDefs::ObjectId, RenderGroupPointer> m_ObjectIdToRenderGroup;
    GlobalShaderUniforms m_GlobalShaderUniforms;

    Profiling::ProfilerModel m_ProfilerModel;
    Profiling::ProfilerItemModelAdatper m_ProfilerItemModelAdapter;
};

#endif // RENDERMODEL_H
