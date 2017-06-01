#include "rendermodestore.h"

#include <functional>

#include "rendersystem/private/rendermode/barebonesrendermode.h"

namespace
{
    // This is an array of std::functions because we don't actually
    // want to store pointers to the programs outside of the store,
    // or this would conflict with the ownership the store is
    // supposed to have over the objects.
    std::function<BaseRenderMode*(void)> g_Initialisers[] =
    {
        [] { return new BarebonesRenderMode(); },
    };

    template <quint32 N>
    struct TypeOfSize
    {
        typedef char type[N];
    };

    template <typename T, quint32 N>
    typename TypeOfSize<N>::type& SizeofArrayHelper(T(&)[N]);
}

RenderModeStore::RenderModeStore()
    : StaticObjectStore<BaseRenderMode*, RenderModeStoreKey>()
{
    using namespace RenderSystem;
    static_assert(sizeof(SizeofArrayHelper(g_Initialisers)) == ShaderDefs::TOTAL_RENDER_MODES,
                  "Initialiser array size mismatch - has a new render mode been added?");

    for ( int i = 0; i < ShaderDefs::TOTAL_RENDER_MODES; ++i )
    {
        m_Objects.insert(static_cast<ShaderDefs::RenderMode>(i), g_Initialisers[i]());
    }
}

void RenderModeStore::storeInitialised()
{

}

void RenderModeStore::storeDestroyed()
{
    qDeleteAll(m_Objects.values());
}
