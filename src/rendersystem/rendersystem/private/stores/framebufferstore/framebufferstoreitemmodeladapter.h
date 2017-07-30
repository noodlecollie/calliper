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
    FrameBufferStoreItemModelAdapter(const FrameBufferStore& store);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual int columnCount(const QModelIndex &parent) const override;

protected:
    virtual QVariant itemData(const ObjectId &id, const QModelIndex &index, int role) const override;

private:
    const FrameBufferStore& m_Store;
};

#endif // FRAMEBUFFERSTOREITEMMODELADAPTER_H
