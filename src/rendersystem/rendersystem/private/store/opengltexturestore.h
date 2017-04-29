#ifndef OPENGLTEXTURESTORE_H
#define OPENGLTEXTURESTORE_H

#include "rendersystem/private/store/objectstore.h"
#include "rendersystem/private/store/objectstoreitempointer.h"
#include "rendersystem/private/store-objects/storedopengltexture.h"

typedef ObjectStoreItemPointer<StoredOpenGLTexture> StoredOpenGLTexturePointer;

class OpenGLTextureStore : public ObjectStore<StoredOpenGLTexturePointer>
{
};

#endif // OPENGLTEXTURESTORE_H
