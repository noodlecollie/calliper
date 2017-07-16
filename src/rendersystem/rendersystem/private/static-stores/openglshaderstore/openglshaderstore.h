#ifndef OPENGLSHADERSTORE_H
#define OPENGLSHADERSTORE_H

#include "containers/static/staticobjectstorearray.h"
#include "rendersystem/private/shaders/base/openglshaderprogram.h"
#include "calliperutil/global/globalinstancehelper.h"

// These are plain shader pointers to indicate they should not be owned or stored by other objects.
class OpenGLShaderStore : public Containers::StaticObjectStoreArray<OpenGLShaderProgram*, RenderSystem::ShaderDefs::TOTAL_SHADERS>,
                          public CalliperUtil::GlobalInstanceHelper<OpenGLShaderStore>
{
public:
    OpenGLShaderStore();
    virtual ~OpenGLShaderStore();

private:
    typedef Containers::StaticObjectStoreArray<OpenGLShaderProgram*, RenderSystem::ShaderDefs::TOTAL_SHADERS> StoreType;
};

#endif // OPENGLSHADERSTORE_H
