#include "irenderer.h"
#include "rendermodel/0-modellevel/rendermodel.h"

namespace Renderer
{
    namespace Global
    {
        IRenderer* g_pRenderer = nullptr;

        void initialise()
        {
            Q_ASSERT_X(!g_pRenderer, Q_FUNC_INFO, "Initialise called when renderer already exists!");
            g_pRenderer = new RenderModel();
        }

        void shutdown()
        {
            Q_ASSERT_X(g_pRenderer, Q_FUNC_INFO, "Shutdown called when renderer doesn't exist!");
            delete g_pRenderer;
            g_pRenderer = nullptr;
        }

        IRenderer* renderer()
        {
            return g_pRenderer;
        }
    }
}
