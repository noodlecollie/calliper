#ifndef OPENGLSHADERSTORE_H
#define OPENGLSHADERSTORE_H

#include "rendersystem/private/store-classes/staticobjectstore.h"
#include "rendersystem/private/shaders/base/openglshaderprogram.h"
#include "rendersystem/private/shaders/common/knownshaderdefs.h"

class OpenGLShaderStore : public StaticObjectStore<OpenGLShaderProgram*, KnownShaderDefs::KnownShaderId, KnownShaderDefs::TOTAL_SHADERS>
{
protected:
    virtual void storeInitialised() override;
    virtual void storeDestroyed() override;
};

#endif // OPENGLSHADERSTORE_H
