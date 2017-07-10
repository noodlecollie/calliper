#include "rendermodel.h"
#include "rendermodel-defs.h"

#include "rendersystem/interface-classes/definitions/materialdefs.h"
#include "rendersystem/private/stores/framebufferstore.h/framebufferstore.h"

// REMOVE ME
#include "calliperutil/opengl/openglhelpers.h"

RenderModel::RenderModel()
    : m_Context(),
      m_RenderGroups(),
      m_ObjectIdToRenderGroup(),
      m_GlobalShaderUniforms(QOpenGLBuffer::DynamicDraw)
{
    m_GlobalShaderUniforms.create();
}

RenderModel::~RenderModel()
{
    clear();
    m_GlobalShaderUniforms.destroy();
}

void RenderModel::setGeometry(const RenderSystem::GeometryBuilder &geometry)
{
    removeGeometry(geometry.objectId());

    for ( int i = 0; i < geometry.sectionCount(); ++i )
    {
        setGeometry(geometry.section(i));
    }
}

void RenderModel::setGeometry(const QSharedPointer<RenderSystem::GeometrySection> &section)
{
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
    QList<RenderGroupPointer> renderGroups = m_ObjectIdToRenderGroup.values(objectId);

    for ( const RenderGroupPointer& group : renderGroups )
    {
        group->removeGeometry(objectId);
    }

    m_ObjectIdToRenderGroup.remove(objectId);
}

void RenderModel::clear()
{
    m_RenderGroups.clear();
    m_ObjectIdToRenderGroup.clear();
}

void RenderModel::draw(RenderSystem::FrameBufferDefs::FrameBufferId frameBufferId,
                       RenderSystem::ShaderDefs::RenderMode renderMode,
                       const QMatrix4x4 &worldToCameraMatrix,
                       const QMatrix4x4 &projectionMatrix)
{
    QSharedPointer<QOpenGLFramebufferObject> frameBufferObject = frameBuffer(frameBufferId);
    if ( !frameBufferObject || !frameBufferObject->bind() )
    {
        return;
    }

    GL_CURRENT_F;
    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_CULL_FACE);
    f->glDepthFunc(GL_LESS);
    f->glCullFace(GL_BACK);
    f->glClearColor(0,0,0,1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Context.setRenderMode(renderMode);

    m_GlobalShaderUniforms.setWorldToCameraMatrix(worldToCameraMatrix);
    m_GlobalShaderUniforms.setProjectionMatrix(projectionMatrix);
    m_GlobalShaderUniforms.setDirectionalLight(QVector3D(1,1,1).normalized());
    m_GlobalShaderUniforms.upload();

    for ( RenderGroupHash::const_iterator itGroup = m_RenderGroups.constBegin();
          itGroup != m_RenderGroups.constEnd();
          ++itGroup )
    {
        itGroup.value()->draw();
    }

    frameBufferObject->release();
}

QSharedPointer<QOpenGLFramebufferObject> RenderModel::frameBuffer(RenderSystem::FrameBufferDefs::FrameBufferId id) const
{
    return FrameBufferStore::globalInstance()->object(id);
}
