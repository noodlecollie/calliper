#ifndef OPENGLTEXTURESTORE_H
#define OPENGLTEXTURESTORE_H

#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"

#include "rendersystem/private/store-classes/itempointerbasedobjectstore.h"
#include "rendersystem/private/store-classes/objectstoreitempointer.h"

#include "namedopengltexture.h"

class OpenGLTextureStore : public ItemPointerBasedObjectStore<NamedOpenGLTexture, RenderSystem::PublicStoreDefs::TextureId>
{
protected:
    virtual void objectCreated(const ObjectId id) override;
    virtual void objectAboutToBeDestroyed(const ObjectId id) override;
};

#endif // OPENGLTEXTURESTORE_H
