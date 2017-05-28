#ifndef OPENGLSHADERSTORE_H
#define OPENGLSHADERSTORE_H

#include "openglshaderstorekey.h"

#include "rendersystem/private/store-classes/staticobjectstore.h"
#include "rendersystem/private/shaders/base/openglshaderprogram.h"
#include "rendersystem/private/store-classes/globalinstancehelper.h"

// These are plain shader pointers to indicate they should not be owned or stored by other objects.
class OpenGLShaderStore : public StaticObjectStore<OpenGLShaderProgram*,
                                                   OpenGLShaderStoreKey>,
                          public GlobalInstanceHelper<OpenGLShaderStore>
{
public:
    OpenGLShaderStore();

protected:
    virtual void storeInitialised() override;
    virtual void storeDestroyed() override;

private:
    static OpenGLShaderStore* m_pGlobalInstance;
};

#endif // OPENGLSHADERSTORE_H
