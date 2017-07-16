#include "framebufferoperations.h"

#include <QSharedPointer>
#include <QImage>

#include "rendersystem/private/stores/framebufferstore/framebufferstore.h"

namespace
{
    inline bool checkStore()
    {
        Q_ASSERT_X(FrameBufferStore::globalInstance(), Q_FUNC_INFO, "Frame buffer store not initialised!");
        return FrameBufferStore::globalInstance() != Q_NULLPTR;
    }
}

void FrameBufferOperations::saveToFile(const RenderSystem::FrameBufferDefs::FrameBufferId id, const QString &filename)
{
    if ( !checkStore() || id == FrameBufferStore::INVALID_ID )
    {
        return;
    }

    QSharedPointer<QOpenGLFramebufferObject> frameBuffer = FrameBufferStore::globalInstance()->object(id);
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

void FrameBufferOperations::drawContentsToCurrent(const RenderSystem::FrameBufferDefs::FrameBufferId id)
{
    if ( !checkStore() || id == FrameBufferStore::INVALID_ID )
    {
        return;
    }

    QSharedPointer<QOpenGLFramebufferObject> frameBuffer = FrameBufferStore::globalInstance()->object(id);
    if ( !frameBuffer )
    {
        return;
    }

    // TODO: Continue from here.
}
