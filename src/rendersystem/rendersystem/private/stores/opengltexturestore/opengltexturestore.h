#ifndef OPENGLTEXTURESTORE_H
#define OPENGLTEXTURESTORE_H

#include "rendersystem/private/store-classes/itempointerbasedobjectstore.h"
#include "rendersystem/private/store-classes/objectstoreitempointer.h"

#include "namedopengltexture.h"

class OpenGLTextureStore : public ItemPointerBasedObjectStore<NamedOpenGLTexture, quint32>
{
protected:
    virtual void objectCreated(const ObjectId id) override;
    virtual void objectAboutToBeDestroyed(const ObjectId id) override;
};

#endif // OPENGLTEXTURESTORE_H
