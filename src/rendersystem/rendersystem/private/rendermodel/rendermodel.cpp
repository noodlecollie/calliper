#include "rendermodel.h"
#include "rendermodel-defs.h"

#include "rendersystem/interface-classes/definitions/materialdefs.h"
#include "rendersystem/private/stores/framebufferstore/framebufferstore.h"
#include "rendersystem/global/rendersystem.h"

#include "calliperutil/opengl/openglhelpers.h"
#include "calliperutil/opengl/openglerrors.h"

#include "profiling/scoped/scopedprofiler.h"

namespace
{
    constexpr quint32 MAX_PROFILER_MODEL_SLOTS = 32;
}

RenderModel::RenderModel(const QString& name)
    : m_strName(name),
      m_Context(),
      m_RenderGroups(),
      m_ObjectIdToRenderGroup(),
      m_GlobalShaderUniforms(QOpenGLBuffer::DynamicDraw),
      m_ProfilerModel(MAX_PROFILER_MODEL_SLOTS),
      m_ProfilerItemModelAdapter(m_ProfilerModel)
{
    Q_ASSERT(RenderSystem::Global::renderSystemContextIsCurrent());
    snapCreationContext();

    m_GlobalShaderUniforms.create();
}

RenderModel::~RenderModel()
{
    verifyCurrentContext();

    clear();
    m_GlobalShaderUniforms.destroy();
}

QString RenderModel::name() const
{
    return m_strName;
}

void RenderModel::setName(const QString &newName)
{
    m_strName = newName;
}

void RenderModel::setGeometry(const RenderSystem::GeometryBuilder &geometry)
{
    verifyCurrentContext();

    removeGeometry(geometry.objectId());

    for ( int i = 0; i < geometry.sectionCount(); ++i )
    {
        setGeometry(geometry.section(i));
    }
}

void RenderModel::setGeometry(const QSharedPointer<RenderSystem::GeometrySection> &section)
{
    verifyCurrentContext();

    if ( section->isEmpty() )
    {
        return;
    }

    RenderSystem::MaterialDefs::MaterialId key = section->materialId();
    RenderGroupPointer renderGroup = m_RenderGroups.value(key, RenderGroupPointer());

    if ( !renderGroup )
    {
        renderGroup = RenderGroupPointer::create(m_Context, key);
        m_RenderGroups.insert(key, renderGroup);
    }

    renderGroup->setGeometry(section);
}

void RenderModel::removeGeometry(RenderSystem::RenderModelDefs::ObjectId objectId)
{
    verifyCurrentContext();

    QList<RenderGroupPointer> renderGroups = m_ObjectIdToRenderGroup.values(objectId);

    for ( const RenderGroupPointer& group : renderGroups )
    {
        group->removeGeometry(objectId);
    }

    m_ObjectIdToRenderGroup.remove(objectId);
}

void RenderModel::clear()
{
    verifyCurrentContext();

    m_RenderGroups.clear();
    m_ObjectIdToRenderGroup.clear();
}

void RenderModel::draw(RenderSystem::FrameBufferDefs::FrameBufferId frameBufferId,
                       const RenderSystem::FrameDrawParams& drawParams)
{
    SCOPED_PROFILER("RenderModel Draw", m_ProfilerModel)

    verifyCurrentContext();

    QSharedPointer<QOpenGLFramebufferObject> frameBufferObject = frameBuffer(frameBufferId);
    if ( !frameBufferObject || !frameBufferObject->bind() )
    {
        return;
    }

    m_Context.setRenderMode(drawParams.renderMode());

    m_GlobalShaderUniforms.setWorldToCameraMatrix(drawParams.worldToCameraMatrix());
    m_GlobalShaderUniforms.setProjectionMatrix(drawParams.projectionMatrix());

    QVector3D directionalLight = drawParams.directionalLight();
    if ( !directionalLight.isNull() )
    {
        directionalLight.normalize();
    }

    m_GlobalShaderUniforms.setDirectionalLight(directionalLight);

    m_GlobalShaderUniforms.upload();

    drawPreFrame(QSize(frameBufferObject->width(), frameBufferObject->height()), drawParams);

    for ( RenderGroupHash::const_iterator itGroup = m_RenderGroups.constBegin();
          itGroup != m_RenderGroups.constEnd();
          ++itGroup )
    {
        itGroup.value()->draw();
    }

    frameBufferObject->release();

    GL_CURRENT_F;
    GLTRY(f->glFinish());
}

void RenderModel::drawPreFrame(const QSize& size, const RenderSystem::FrameDrawParams &drawParams)
{
    GL_CURRENT_F;

    // This is needed so that drawing into the frame buffer happens in the right place!
    GLTRY(f->glViewport(0, 0, size.width(), size.height()));

    const QColor clearColor = drawParams.backgroundColor();
    GLTRY(f->glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF()));

    GLTRY(f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

QSharedPointer<QOpenGLFramebufferObject> RenderModel::frameBuffer(RenderSystem::FrameBufferDefs::FrameBufferId id) const
{
    return FrameBufferStore::globalInstance()->object(id);
}

Profiling::ProfilerItemModelAdatper* RenderModel::profilingData()
{
    return &m_ProfilerItemModelAdapter;
}
