#include "framebufferstoreitemmodeladapter.h"

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
        case ObjectIdColumn:
        {
            return QVariant::fromValue(id);
        }

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
    if ( role != Qt::DisplayRole || orientation != Qt::Horizontal )
    {
        return objectStoreAbstractItemModel()->defaultHeaderData(section, orientation, role);
    }

    if ( section < ObjectIdColumn || section >= TOTAL_MODEL_COLUMNS )
    {
        return objectStoreAbstractItemModel()->defaultHeaderData(section, orientation, role);
    }

    switch ( section )
    {
        case ObjectIdColumn:
        {
            return QCoreApplication::translate("FrameBufferStoreItemModelAdapter", "Object ID");
        }

        case FBOHandleColumn:
        {
            return QCoreApplication::translate("FrameBufferStoreItemModelAdapter", "OpenGL Handle");
        }

        case SizeColumn:
        {
            return QCoreApplication::translate("FrameBufferStoreItemModelAdapter", "Size");
        }

        default:
        {
            return objectStoreAbstractItemModel()->defaultHeaderData(section, orientation, role);
        }
    }
}
