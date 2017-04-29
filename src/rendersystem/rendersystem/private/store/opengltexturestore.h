#ifndef OPENGLTEXTURESTORE_H
#define OPENGLTEXTURESTORE_H

#include "rendersystem_global.h"

#include "rendersystem/interface-classes/store/objectstore.h"
#include "rendersystem/interface-classes/store/objectstoreitempointer.h"
#include "rendersystem/interface-classes/store-objects/storedopengltexture.h"

namespace RenderSystem
{
    typedef ObjectStoreItemPointer<StoredOpenGLTexture> StoredOpenGLTexturePointer;

    class OpenGLTextureStore : public ObjectStore<StoredOpenGLTexturePointer>
    {
    };
}

#endif // OPENGLTEXTURESTORE_H
