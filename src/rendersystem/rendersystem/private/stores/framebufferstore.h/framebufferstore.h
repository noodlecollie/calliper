#ifndef FRAMEBUFFERSTORE_H
#define FRAMEBUFFERSTORE_H

#include <QOpenGLFramebufferObject>

#include "rendersystem/interfaces/iframebufferstore.h"
#include "rendersystem/interface-classes/definitions/framebufferdefs.h"

#include "containers/itempointer/itempointerbasedobjectstore.h"

#include "calliperutil/global/globalinstancehelper.h"

class FrameBufferStore : public Containers::ItemPointerBasedObjectStore<QOpenGLFramebufferObject,
                                                                        RenderSystem::FrameBufferDefs::FrameBufferId>,
                         public RenderSystem::IFrameBufferStore,
                         public CalliperUtil::GlobalInstanceHelper<FrameBufferStore>
{
public:
    typedef RenderSystem::FrameBufferDefs::FrameBufferId FrameBufferId;

    FrameBufferStore();
    virtual ~FrameBufferStore();

    virtual FrameBufferId createFrameBuffer(const QSize& size) override;
    virtual void removeFrameBuffer(const FrameBufferId id) override;
    virtual void resizeFrameBuffer(const FrameBufferId id, const QSize& newSize) override;
    virtual bool frameBufferExists(const FrameBufferId id) const override;
    virtual GLuint frameBufferTextureId(const FrameBufferId id) const override;
};

#endif // FRAMEBUFFERSTORE_H
