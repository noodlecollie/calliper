#ifndef OPENGLBACKGROUNDCONTEXT_H
#define OPENGLBACKGROUNDCONTEXT_H

#include "renderer_global.h"

#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QLoggingCategory>
#include <QOpenGLFunctions_4_1_Core>

namespace Renderer
{
    Q_DECLARE_LOGGING_CATEGORY(lcMainRenderContext)

    class RENDERERSHARED_EXPORT MainRenderContext
    {
    public:
        MainRenderContext();
        ~MainRenderContext();

        bool create();
        bool isCreated() const;

        QOpenGLContext* context() const;
        QOffscreenSurface* surface() const;

        bool makeCurrent();
        void doneCurrent();

        QOpenGLFunctions_4_1_Core* versionFunctions() const;

        static void globalInitialise();
        static void globalShutdown();
        static MainRenderContext* globalInstance();

    private:
        static MainRenderContext* m_pGlobalInstance;

        QOffscreenSurface* m_pSurface;
        QOpenGLContext* m_pContext;
        bool m_bCreatedSuccessfully;
    };
}

#endif // OPENGLBACKGROUNDCONTEXT_H
