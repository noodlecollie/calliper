#include "sceneframerenderer.h"

#include "calliperutil/debug/debug.h"

#include "model/scenerenderer/scenerenderer.h"
#include "model/scene/mapscene.h"
#include "model/global/resourceenvironment.h"

#include "renderer/global/mainrendercontext.h"
#include "renderer/opengl/scopedcurrentcontext.h"

namespace Model
{
    SceneFrameRenderer::SceneFrameRenderer()
        : m_nNextFrameBufferId(0),
          m_FrameBufferHash()
    {

    }

    SceneFrameRenderer::~SceneFrameRenderer()
    {
    }

    SceneFrameRenderer::FrameBufferId SceneFrameRenderer::createFrameBuffer(const QSize &size)
    {
        Q_ASSERT_X(m_nNextFrameBufferId + 1 > 0, Q_FUNC_INFO, "Frame buffer ID overflow!");
        FrameBufferId newId = m_nNextFrameBufferId++;

        m_FrameBufferHash.insert(newId, FrameBufferPointer::create(size));
        return newId;
    }

    void SceneFrameRenderer::destroyFrameBuffer(FrameBufferId id)
    {
        m_FrameBufferHash.remove(id);
    }

    void SceneFrameRenderer::resizeFrameBuffer(FrameBufferId id, const QSize &newSize)
    {
        if ( !frameBufferValid(id) )
        {
            return;
        }

        m_FrameBufferHash.remove(id);
        m_FrameBufferHash.insert(id, FrameBufferPointer::create(newSize));
    }

    bool SceneFrameRenderer::frameBufferValid(FrameBufferId id) const
    {
        return m_FrameBufferHash.contains(id);
    }

    void SceneFrameRenderer::renderScene(FrameBufferId id, MapScene *scene, Renderer::RenderModel *renderModel)
    {
        FrameBufferPointer frameBuffer = m_FrameBufferHash.value(id, FrameBufferPointer());
        if ( frameBuffer.isNull() )
        {
            return;
        }

        CUTL_ASSERT_SUCCESS(frameBuffer->bind());

        SceneRenderer sceneRenderer(scene, renderModel, frameBuffer.data());
        sceneRenderer.setShaderPalette(ResourceEnvironment::globalInstance()->shaderPaletteStore()
                                       ->shaderPalette(ShaderPaletteStore::SimpleLitTexturedRenderMode));
        sceneRenderer.render(scene->defaultCamera());

        CUTL_ASSERT_SUCCESS(frameBuffer->release());
    }

    GLuint SceneFrameRenderer::frameBufferTextureId(FrameBufferId id) const
    {
        FrameBufferPointer frameBuffer = m_FrameBufferHash.value(id, FrameBufferPointer());
        if ( frameBuffer.isNull() )
        {
            return 0;
        }

        return frameBuffer->texture();
    }
}
