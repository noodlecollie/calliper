#include "framebufferstore.h"

// REMOVE ME
#include <QImage>

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

void FrameBufferStore::removeFrameBuffer(const FrameBufferId id)
{
    if ( id == INVALID_ID )
    {
        return;
    }

    destroy(id);
}

bool FrameBufferStore::frameBufferExists(const FrameBufferId id) const
{
    if ( id == INVALID_ID )
    {
        return false;
    }

    return contains(id);
}

GLuint FrameBufferStore::frameBufferTextureId(const FrameBufferId id) const
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

QSize FrameBufferStore::frameBufferSize(const FrameBufferId id) const
{
    if ( id == INVALID_ID )
    {
        return QSize();
    }

    QSharedPointer<QOpenGLFramebufferObject> frameBuffer = object(id);
    if ( !frameBuffer )
    {
        return QSize();
    }

    return frameBuffer->size();
}

QSharedPointer<QOpenGLFramebufferObject> FrameBufferStore::frameBuffer(const FrameBufferId id) const
{
    return object(id);
}

void FrameBufferStore::setFrameBufferSize(const FrameBufferId id, const QSize size)
{
    if ( id == INVALID_ID )
    {
        return;
    }

    replace(id, size);
}

// REMOVE ME
void FrameBufferStore::save(const FrameBufferId id, const QString &filename)
{
    if ( id == INVALID_ID )
    {
        return;
    }

    QSharedPointer<QOpenGLFramebufferObject> frameBuffer = object(id);
    if ( !frameBuffer )
    {
        return;
    }

    QImage image = frameBuffer->toImage();
    if ( image.isNull() )
    {
        return;
    }

    image.save(filename);
}