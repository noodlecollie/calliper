#include "openglshaderstore.h"

#include <functional>

#include "calliperutil/array/arrayutil.h"
#include "calliperutil/opengl/openglerrors.h"

#include "rendersystem/private/shaders/derived/errorshader.h"
#include "rendersystem/private/shaders/derived/litgenericshader.h"
#include "rendersystem/private/shaders/derived/unlitgenericshader.h"
#include "rendersystem/private/shaders/derived/debugminimalshader.h"

namespace
{
    template<typename T>
    inline OpenGLShaderProgram* create()
    {
        T* program = new T();
        program->create();
        program->construct();
        return program;
    }

    OpenGLShaderStore::InitialiserFunction initialisers[] =
    {
        [](){ return create<ErrorShader>(); },
        [](){ return create<LitGenericShader>(); },
        [](){ return create<UnlitGenericShader>(); },
        [](){ return create<DebugMinimalShader>(); },
    };
}

OpenGLShaderStore::OpenGLShaderStore()
    : StoreType()
{
    initialise(initialisers);
}

OpenGLShaderStore::~OpenGLShaderStore()
{
    for ( quint32 i = 0; i < RenderSystem::ShaderDefs::TOTAL_SHADERS; ++i )
    {
        delete m_ObjectArray[i];
        m_ObjectArray[i] = Q_NULLPTR;
    }
}
