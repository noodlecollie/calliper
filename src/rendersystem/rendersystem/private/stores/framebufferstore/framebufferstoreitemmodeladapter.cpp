#include "framebufferstoreitemmodeladapter.h"

#include <QApplication>

#include "rendersystem/private/stores/framebufferstore/framebufferstore.h"

FrameBufferStoreItemModelAdapter::FrameBufferStoreItemModelAdapter(const FrameBufferStore &store)
    : Containers::ItemPointerBasedItemModelAdapter<QOpenGLFramebufferObject,
                                                   RenderSystem::FrameBufferDefs::FrameBufferId>(&store),
      m_Store(store)
{

}

QVariant FrameBufferStoreItemModelAdapter::itemData(const ObjectId &id, int role) const
{
    QSharedPointer<QOpenGLFramebufferObject> fbo = m_Store.frameBuffer(id);
    if ( !fbo )
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Expected valid frame buffer!");
        return QVariant();
    }

    if ( role != Qt::DisplayRole )
    {
        return QVariant();
    }

    const QString text = QString("FBO %0 [%1x%2] (%3, %4)")
            .arg(id)
            .arg(fbo->width())
            .arg(fbo->height())
            .arg(fbo->isValid() ? QApplication::translate("FrameBufferStoreItemModelAdapter", "Valid")
                                : QApplication::translate("FrameBufferStoreItemModelAdapter", "Invalid"))
            .arg(fbo->isBound() ? QApplication::translate("FrameBufferStoreItemModelAdapter", "Bound")
                                : QApplication::translate("FrameBufferStoreItemModelAdapter", "Unbound"));

    return QVariant(text);
}
