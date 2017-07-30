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

    const QString text = QString("FBO %0 [%1x%2]")
            .arg(fbo->handle())
            .arg(fbo->width())
            .arg(fbo->height());

    return QVariant(text);
}

QVariant FrameBufferStoreItemModelAdapter::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( orientation != Qt::Horizontal || section != 0 || role != Qt::DisplayRole )
    {
        return abstractItemModel()->defaultHeaderData(section, orientation, role);
    }

    return QApplication::translate("FrameBufferStoreItemModelAdapter", "Frame Buffer Object");
}
