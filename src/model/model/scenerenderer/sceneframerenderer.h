#ifndef SCENEFRAMERENDERER_H
#define SCENEFRAMERENDERER_H

#include "model_global.h"

#include <QSize>
#include <QOpenGLFramebufferObject>
#include <QHash>
#include <QSharedPointer>

namespace Renderer
{
    class RenderModel;
}

namespace Model
{
    class MapScene;

    class MODELSHARED_EXPORT SceneFrameRenderer
    {
    public:
        typedef quint16 FrameBufferId;

        SceneFrameRenderer();
        ~SceneFrameRenderer();

        FrameBufferId createFrameBuffer(const QSize& size);
        void destroyFrameBuffer(FrameBufferId id);
        void resizeFrameBuffer(FrameBufferId id, const QSize& newSize);
        bool frameBufferValid(FrameBufferId id) const;

        void renderScene(FrameBufferId id, MapScene* scene, Renderer::RenderModel* renderModel);
        GLuint frameBufferTextureId(FrameBufferId id) const;

    private:
        typedef QSharedPointer<QOpenGLFramebufferObject> FrameBufferPointer;

        quint16 m_nNextFrameBufferId;
        QHash<FrameBufferId, FrameBufferPointer> m_FrameBufferHash;
    };
}

#endif // SCENEFRAMERENDERER_H
