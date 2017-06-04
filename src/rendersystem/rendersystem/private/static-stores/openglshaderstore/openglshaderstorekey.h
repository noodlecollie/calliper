#ifndef OPENGLSHADERSTOREKEY_H
#define OPENGLSHADERSTOREKEY_H

#include "containers/static/staticobjectstorekey.h"
#include "rendersystem/private/shaders/common/privateshaderdefs.h"

class OpenGLShaderStoreKey : public Containers::StaticObjectStoreKey<PrivateShaderDefs::ShaderId,
                                                                     PrivateShaderDefs::TOTAL_SHADERS>
{
};

#endif // OPENGLSHADERSTOREKEY_H
