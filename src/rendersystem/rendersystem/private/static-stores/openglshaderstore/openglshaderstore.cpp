#include "openglshaderstore.h"

#include <functional>

#include "calliperutil/array/arrayutil.h"
#include "calliperutil/opengl/openglerrors.h"

#include "rendersystem/private/shaders/derived/errorshader.h"
#include "rendersystem/private/shaders/derived/simplelitshader.h"
#include "rendersystem/private/shaders/derived/unlitpervertexcolorshader.h"
#include "rendersystem/private/shaders/derived/debugminimalshader.h"
#include "rendersystem/private/shaders/derived/screenspacequadshader.h"

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
        [](){ return create<SimpleLitShader>(); },
        [](){ return create<UnlitPerVertexColorShader>(); },
        [](){ return create<DebugMinimalShader>(); },
        [](){ return create<ScreenSpaceQuadShader>(); },
    };
}

OpenGLShaderStore::OpenGLShaderStore()
    : StoreType()
{
    initialise(initialisers);
}

OpenGLShaderStore::~OpenGLShaderStore()
{
    for ( quint32 i = 0; i < PrivateShaderDefs::TOTAL_SHADERS; ++i )
    {
        delete m_ObjectArray[i];
        m_ObjectArray[i] = Q_NULLPTR;
    }
}
