#include "framebufferstore.h"

FrameBufferStore::FrameBufferStore()
    : ItemPointerBasedObjectStore<QOpenGLFramebufferObject,
                                  RenderSystem::FrameBufferDefs::FrameBufferId>()
{

}

FrameBufferStore::~FrameBufferStore()
{

}

FrameBufferStore::FrameBufferId FrameBufferStore::createFrameBuffer(const QSize& size)
{
    if ( size.isEmpty() || size.isNull() )
    {
        return INVALID_ID;
    }

    return create(size);
}

void FrameBufferStore::removeFrameBuffer(const FrameBufferStore::FrameBufferId id)
{
    if ( id == INVALID_ID )
    {
        return;
    }

    destroy(id);
}

void FrameBufferStore::resizeFrameBuffer(const FrameBufferStore::FrameBufferId id, const QSize& newSize)
{
    if ( id == INVALID_ID || !frameBufferExists(id) )
    {
        return;
    }

    replace(id, newSize);
}

bool FrameBufferStore::frameBufferExists(const FrameBufferStore::FrameBufferId id) const
{
    if ( id == INVALID_ID )
    {
        return false;
    }

    return contains(id);
}

GLuint FrameBufferStore::frameBufferTextureId(const FrameBufferStore::FrameBufferId id) const
{
    if ( id == INVALID_ID )
    {
        return 0;
    }

    QSharedPointer<QOpenGLFramebufferObject> frameBuffer = object(id);
    if ( !frameBuffer )
    {
        return 0;
    }

    return frameBuffer->texture();
}
