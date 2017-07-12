#include "rendermodel.h"
#include "rendermodel-defs.h"

#include "rendersystem/interface-classes/definitions/materialdefs.h"
#include "rendersystem/private/stores/framebufferstore.h/framebufferstore.h"

// REMOVE ME
#include "calliperutil/opengl/openglhelpers.h"
#include <QOpenGLShaderProgram>
#include "rendersystem/private/opengl/openglvertexarrayobject.h"
#include <QOpenGLBuffer>
#include <QImage>
#include "rendersystem/private/static-stores/openglshaderstore/openglshaderstore.h"

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
                       const RenderSystem::FrameDrawParams& drawParams)
{
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

    drawPreFrame(*frameBufferObject, drawParams);

    for ( RenderGroupHash::const_iterator itGroup = m_RenderGroups.constBegin();
          itGroup != m_RenderGroups.constEnd();
          ++itGroup )
    {
        itGroup.value()->draw();
    }

    frameBufferObject->release();
}

void RenderModel::drawPreFrame(QOpenGLFramebufferObject &frameBuffer, const RenderSystem::FrameDrawParams &drawParams)
{
    GL_CURRENT_F;

    // This is needed so that drawing into the frame buffer happens in the right place!
    f->glViewport(0, 0, frameBuffer.width(), frameBuffer.height());

    const QColor clearColor = drawParams.backgroundColor();
    f->glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

QSharedPointer<QOpenGLFramebufferObject> RenderModel::frameBuffer(RenderSystem::FrameBufferDefs::FrameBufferId id) const
{
    return FrameBufferStore::globalInstance()->object(id);
}
