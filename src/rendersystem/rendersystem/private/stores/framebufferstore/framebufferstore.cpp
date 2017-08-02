#include "framebufferstore.h"

#include <QOpenGLFramebufferObject>

FrameBufferStore::FrameBufferStore()
    : ItemPointerBasedObjectStore<QOpenGLFramebufferObject,
                                  RenderSystem::FrameBufferDefs::FrameBufferId>(),
      m_ItemModelAdapter(*this)
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

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);

    return create(size, format);
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

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);

    replace(id, size, format);
}

QAbstractItemModel* FrameBufferStore::itemModel() const
{
    return m_ItemModelAdapter.abstractItemModel();
}
