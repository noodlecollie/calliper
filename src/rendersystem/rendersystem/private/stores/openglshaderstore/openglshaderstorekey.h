#ifndef OPENGLSHADERSTOREKEY_H
#define OPENGLSHADERSTOREKEY_H

#include "rendersystem/private/store-classes/staticobjectstorekey.h"
#include "rendersystem/private/shaders/common/privateshaderdefs.h"

class OpenGLShaderStoreKey : public StaticObjectStoreKey<PrivateShaderDefs::ShaderId,
                                                         PrivateShaderDefs::TOTAL_SHADERS>
{
};

#endif // OPENGLSHADERSTOREKEY_H
