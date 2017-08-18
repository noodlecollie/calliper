#ifndef FRAMEBUFFERSTOREITEMMODELADAPTER_H
#define FRAMEBUFFERSTOREITEMMODELADAPTER_H

#include <QOpenGLFramebufferObject>
#include "rendersystem/interface-classes/definitions/framebufferdefs.h"

#include "containers/adapters/itempointerbaseditemmodeladapter.h"

class FrameBufferStore;

class FrameBufferStoreItemModelAdapter : public Containers::ItemPointerBasedItemModelAdapter<QOpenGLFramebufferObject,
                                                                                             RenderSystem::FrameBufferDefs::FrameBufferId>
{
public:
    // Columns begin from 1, since the object ID column is always 0.
    enum ModelColumn
    {
        FBOHandleColumn = 1,
        SizeColumn,

        TOTAL_MODEL_COLUMNS
    };

    FrameBufferStoreItemModelAdapter(const FrameBufferStore& store);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual int columnCount(const QModelIndex &parent) const override;

protected:
    virtual QVariant itemData(const ObjectId &id, const QModelIndex &index, int role) const override;

private:
    typedef Containers::ItemPointerBasedItemModelAdapter<QOpenGLFramebufferObject, RenderSystem::FrameBufferDefs::FrameBufferId> BaseClass;

    const FrameBufferStore& m_Store;
};

#endif // FRAMEBUFFERSTOREITEMMODELADAPTER_H
