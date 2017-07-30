#ifndef FRAMEBUFFERSTORE_H
#define FRAMEBUFFERSTORE_H

#include <QOpenGLFramebufferObject>

#include "framebufferstoreitemmodeladapter.h"

#include "rendersystem/interfaces/iframebufferstore.h"
#include "rendersystem/interface-classes/definitions/framebufferdefs.h"

#include "containers/itempointer/itempointerbasedobjectstore.h"

#include "calliperutil/global/globalinstancehelper.h"

class FrameBufferStore : public Containers::ItemPointerBasedObjectStore<QOpenGLFramebufferObject,
                                                                        RenderSystem::FrameBufferDefs::FrameBufferId>,
                         public RenderSystem::IFrameBufferStore,
                         public CalliperUtil::GlobalInstanceHelper<FrameBufferStore>
{
public:
    typedef RenderSystem::FrameBufferDefs::FrameBufferId FrameBufferId;

    FrameBufferStore();
    virtual ~FrameBufferStore();

    // External
    virtual FrameBufferId createFrameBuffer(const QSize& size) override;
    virtual void removeFrameBuffer(const FrameBufferId id) override;
    virtual bool frameBufferExists(const FrameBufferId id) const override;
    virtual GLuint frameBufferTextureId(const FrameBufferId id) const override;
    virtual QSize frameBufferSize(const FrameBufferStore::FrameBufferId id) const override;
    virtual void setFrameBufferSize(const FrameBufferStore::FrameBufferId id, const QSize size) override;
    virtual QAbstractItemModel* itemModel() const override;

    // Internal
    QSharedPointer<QOpenGLFramebufferObject> frameBuffer(const FrameBufferStore::FrameBufferId id) const;

private:
    mutable FrameBufferStoreItemModelAdapter m_ItemModelAdapter;
};

#endif // FRAMEBUFFERSTORE_H
