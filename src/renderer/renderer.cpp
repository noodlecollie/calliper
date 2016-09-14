#include "irenderer.h"
#include "rendermodel/0-modellevel/rendermodel.h"

namespace NS_RENDERER
{
    IRenderer* g_pRenderer = NULL;

    void initialise()
    {
        Q_ASSERT_X(!g_pRenderer, Q_FUNC_INFO, "Initialise called when renderer already exists!");
        g_pRenderer = new RenderModel();
    }

    void shutdown()
    {
        Q_ASSERT_X(g_pRenderer, Q_FUNC_INFO, "Shutdown called when renderer doesn't exist!");
        delete g_pRenderer;
        g_pRenderer = NULL;
    }

    IRenderer* renderer()
    {
        return g_pRenderer;
    }
}
