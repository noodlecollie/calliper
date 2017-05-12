#include "openglshaderstore.h"

#include <functional>

#include "rendersystem/private/shaders/derived/simplelitshader.h"
#include "rendersystem/private/shaders/derived/unlitpervertexcolorshader.h"

namespace
{
    std::function<OpenGLShaderProgram*()> g_Initialisers[] =
    {
        [] { return new SimpleLitShader(); },
        [] { return new UnlitPerVertexColorShader(); },
    };

    template <quint32 N>
    struct TypeOfSize
    {
        typedef char type[N];
    };

    template <typename T, quint32 N>
    typename TypeOfSize<N>::type& SizeofArrayHelper(T(&)[N]);
}

OpenGLShaderStore::OpenGLShaderStore()
    : StaticObjectStore<OpenGLShaderProgram*, PrivateShaderDefs::ShaderId, PrivateShaderDefs::TOTAL_SHADERS>()
{

}

void OpenGLShaderStore::storeInitialised()
{
    static_assert(sizeof(SizeofArrayHelper(g_Initialisers)) == PrivateShaderDefs::TOTAL_SHADERS,
                  "Initialiser array size mismatch - has a new shader ID been added?");

    for ( int i = 0; i < PrivateShaderDefs::TOTAL_SHADERS; ++i )
    {
        m_Objects.insert(static_cast<PrivateShaderDefs::ShaderId>(i), g_Initialisers[i]());
    }
}

void OpenGLShaderStore::storeDestroyed()
{
    qDeleteAll(m_Objects.values());
}

