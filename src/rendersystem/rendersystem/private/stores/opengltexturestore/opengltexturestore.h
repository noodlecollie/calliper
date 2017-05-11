#ifndef OPENGLTEXTURESTORE_H
#define OPENGLTEXTURESTORE_H

#include <QHash>

#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"
#include "rendersystem/interfaces/itexturestore.h"

#include "rendersystem/private/store-classes/itempointerbasedobjectstore.h"
#include "rendersystem/private/store-classes/objectstoreitempointer.h"

#include "namedopengltexture.h"

class OpenGLTextureStore : public ItemPointerBasedObjectStore<NamedOpenGLTexture, RenderSystem::PublicStoreDefs::TextureId>,
                           public RenderSystem::ITextureStore
{
public:
    typedef RenderSystem::PublicStoreDefs::TextureId TextureId;

    virtual TextureId addTexture(const QImage& image, const QString& path) override;
    virtual void removeTexture(const TextureId id) override;
    virtual bool containsTexture(const TextureId id) const override;
    virtual TextureId textureIdFromPath(const QString& path) const override;
    virtual QString texturePathFromId(const TextureId id) const override;

protected:
    virtual void objectCreated(const ObjectId id) override;
    virtual void objectAboutToBeDestroyed(const ObjectId id) override;

private:
    QHash<QString, TextureId> m_PathToTextureIdMap;
};

#endif // OPENGLTEXTURESTORE_H
