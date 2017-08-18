#include "framebufferstoreitemmodeladapter.h"

#include <QApplication>

#include "rendersystem/private/stores/framebufferstore/framebufferstore.h"

FrameBufferStoreItemModelAdapter::FrameBufferStoreItemModelAdapter(const FrameBufferStore &store)
    : Containers::ItemPointerBasedItemModelAdapter<QOpenGLFramebufferObject,
                                                   RenderSystem::FrameBufferDefs::FrameBufferId>(&store),
      m_Store(store)
{

}

int FrameBufferStoreItemModelAdapter::columnCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
    {
        return 0;
    }

    return TOTAL_MODEL_COLUMNS;
}

QVariant FrameBufferStoreItemModelAdapter::itemData(const ObjectId &id, const QModelIndex &index, int role) const
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

    switch ( index.column() )
    {
        case FBOHandleColumn:
        {
            return QVariant(fbo->handle());
        }

        case SizeColumn:
        {
            return QVariant(QString("%0x%1").arg(fbo->width()).arg(fbo->height()));
        }

        default:
        {
            break;
        }
    }

    return QVariant();
}

QVariant FrameBufferStoreItemModelAdapter::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role != Qt::DisplayRole )
    {
        return abstractItemModel()->defaultHeaderData(section, orientation, role);
    }

    if ( orientation != Qt::Horizontal )
    {
        return abstractItemModel()->defaultHeaderData(section, orientation, role);
    }

    if ( section < 0 || section >= TOTAL_MODEL_COLUMNS )
    {
        return abstractItemModel()->defaultHeaderData(section, orientation, role);
    }

    switch ( section )
    {
        case FBOHandleColumn:
        {
            return QApplication::translate("FrameBufferStoreItemModelAdapter", "OpenGL Handle");
        }

        case SizeColumn:
        {
            return QApplication::translate("FrameBufferStoreItemModelAdapter", "Size");
        }

        default:
        {
            return abstractItemModel()->defaultHeaderData(section, orientation, role);
        }
    }
}
