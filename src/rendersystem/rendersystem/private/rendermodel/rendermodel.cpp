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

#define DRAW_OVERRIDE 0

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

    // This is needed so that drawing into the frame buffer happens in the right place!
    GL_CURRENT_F;
    f->glViewport(0, 0, frameBufferObject->width(), frameBufferObject->height());

#if DRAW_OVERRIDE
    static const float vertexData[] =
    {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        0.0f, 1.0f,
    };

    static const quint32 indexData[] =
    {
        0, 1, 2,
    };

    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_CULL_FACE);
    f->glDepthFunc(GL_LESS);
    f->glCullFace(GL_BACK);
    f->glClearColor(0,0,0,1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    OpenGLShaderProgram& program = *OpenGLShaderStore::globalInstance()->object(PrivateShaderDefs::DebugMinimalShaderId);
    program.bind();

    OpenGLVertexArrayObject vao;
    vao.create();

    QOpenGLBuffer vertexBuffer(QOpenGLBuffer::VertexBuffer);
    vertexBuffer.create();

    QOpenGLBuffer indexBuffer(QOpenGLBuffer::IndexBuffer);
    indexBuffer.create();

    vao.bind();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertexData, 6 * sizeof(float));

    indexBuffer.bind();
    indexBuffer.allocate(indexData, 3 * sizeof(quint32));

    program.enableAttributeArray(0);
    program.setAttributeBuffer(0, GL_FLOAT, 0, 2);

    f->glDrawArrays(GL_TRIANGLES, 0, 3);

    indexBuffer.release();
    vertexBuffer.release();
    vao.release();
    indexBuffer.destroy();
    vertexBuffer.destroy();

    program.release();
#else
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
#endif

    frameBufferObject->release();
}

QSharedPointer<QOpenGLFramebufferObject> RenderModel::frameBuffer(RenderSystem::FrameBufferDefs::FrameBufferId id) const
{
    return FrameBufferStore::globalInstance()->object(id);
}
