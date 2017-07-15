#include "rendermodestore.h"

#include <functional>

#include "rendersystem/private/rendermode/barebonesrendermode.h"
#include "rendersystem/private/rendermode/debugminimalrendermode.h"

namespace
{
    RenderModeStore::InitialiserFunction initialisers[] =
    {
        [](){ return new BarebonesRenderMode(); },
        [](){ return new DebugMinimalRenderMode(); },
    };
}

RenderModeStore::RenderModeStore()
    : StoreType()
{
    initialise(initialisers);
}

RenderModeStore::~RenderModeStore()
{
    for ( int i = 0; i < RenderSystem::ShaderDefs::TOTAL_RENDER_MODES; ++i )
    {
        delete m_ObjectArray[i];
        m_ObjectArray[i] = Q_NULLPTR;
    }
}
