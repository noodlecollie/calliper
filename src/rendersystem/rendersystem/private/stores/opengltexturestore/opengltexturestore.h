#ifndef OPENGLTEXTURESTORE_H
#define OPENGLTEXTURESTORE_H

#include <QHash>

#include "namedopengltexture.h"

#include "rendersystem/interface-classes/store/publicstoredefs.h"
#include "rendersystem/interfaces/itexturestore.h"

#include "rendersystem/private/store-classes/pathmanagingobjectstore.h"
#include "rendersystem/private/store-classes/globalinstancehelper.h"

class OpenGLTextureStore : public PathManagingObjectStore<NamedOpenGLTexture, RenderSystem::PublicStoreDefs::TextureId>,
                           public RenderSystem::ITextureStore,
                           public GlobalInstanceHelper<OpenGLTextureStore>
{
public:
    typedef RenderSystem::PublicStoreDefs::TextureId TextureId;

    OpenGLTextureStore();
    virtual ~OpenGLTextureStore();

    virtual TextureId addTexture(const QImage& image, const QString& path) override;
    virtual void removeTexture(const TextureId id) override;
    virtual bool containsTexture(const TextureId id) const override;
    virtual bool containsTexture(const QString& path) const override;
    virtual TextureId textureIdFromPath(const QString& path) const override;
    virtual QString texturePathFromId(const TextureId id) const override;

protected:
    typedef PathManagingObjectStore<NamedOpenGLTexture, RenderSystem::PublicStoreDefs::TextureId> BasePathManagingObjectStore;

    virtual void objectCreated(const ObjectId id) override;
    virtual void objectAboutToBeDestroyed(const ObjectId id) override;

private:
    static OpenGLTextureStore* m_pGlobalInstance;

    void createDefaultTexture();
};

#endif // OPENGLTEXTURESTORE_H
