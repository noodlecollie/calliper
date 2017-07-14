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
    // This is an array of std::functions because we don't actually
    // want to store pointers to the programs outside of the store,
    // or this would conflict with the ownership the store is
    // supposed to have over the objects.
    // Shaders should be added in the same order as the IDs enum.
    std::function<OpenGLShaderProgram*(void)> g_Initialisers[] =
    {
        [] { return new ErrorShader(); },
        [] { return new SimpleLitShader(); },
        [] { return new UnlitPerVertexColorShader(); },
        [] { return new DebugMinimalShader(); },
        [] { return new ScreenSpaceQuadShader(); }
    };
}

OpenGLShaderStore::OpenGLShaderStore()
    : StaticObjectStore<OpenGLShaderProgram*, OpenGLShaderStoreKey>()
{
    static_assert(SIZEOF_ARRAY(g_Initialisers) == PrivateShaderDefs::TOTAL_SHADERS,
                  "Initialiser array size mismatch - has a new shader ID been added?");

    for ( int i = 0; i < PrivateShaderDefs::TOTAL_SHADERS; ++i )
    {
        m_Objects.insert(static_cast<PrivateShaderDefs::ShaderId>(i), g_Initialisers[i]());
    }
}

void OpenGLShaderStore::onStoreInitialised()
{
    for ( StaticObjectStoreHash::iterator itShader = m_Objects.begin();
          itShader != m_Objects.end();
          ++itShader )
    {
        bool creationSuccess = false;
        GLTRY(creationSuccess = itShader.value()->create());
        Q_ASSERT_X(creationSuccess, Q_FUNC_INFO, "Could not create shader!");

        itShader.value()->construct();
    }
}

void OpenGLShaderStore::onStoreDestroyed()
{
    qDeleteAll(m_Objects.values());
}

