#ifndef OPENGLSHADERSTORE_H
#define OPENGLSHADERSTORE_H

#include "rendersystem/private/store-classes/staticobjectstore.h"
#include "rendersystem/private/shaders/base/openglshaderprogram.h"
#include "rendersystem/private/shaders/common/privateshaderdefs.h"

class OpenGLShaderStore : public StaticObjectStore<OpenGLShaderProgram*, PrivateShaderDefs::ShaderId, PrivateShaderDefs::TOTAL_SHADERS>
{
public:
    OpenGLShaderStore();

protected:
    virtual void storeInitialised() override;
    virtual void storeDestroyed() override;
};

#endif // OPENGLSHADERSTORE_H
