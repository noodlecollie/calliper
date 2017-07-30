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

protected:
    virtual QVariant itemData(const ObjectId& id, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    const FrameBufferStore& m_Store;
};

#endif // FRAMEBUFFERSTOREITEMMODELADAPTER_H
