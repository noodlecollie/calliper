#ifndef OPENGLBACKGROUNDCONTEXT_H
#define OPENGLBACKGROUNDCONTEXT_H

#include "renderer_global.h"

#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QLoggingCategory>

namespace Renderer
{
    Q_DECLARE_LOGGING_CATEGORY(lcOpenGLBackgroundContext)

    class RENDERERSHARED_EXPORT OpenGLBackgroundContext
    {
    public:
        OpenGLBackgroundContext();
        ~OpenGLBackgroundContext();

        bool create();
        bool isCreated() const;

        QOpenGLContext* context() const;
        QOffscreenSurface* surface() const;

        bool makeCurrent();
        void doneCurrent();

        static void globalInitialise();
        static void globalShutdown();
        static OpenGLBackgroundContext* globalInstance();

    private:
        static OpenGLBackgroundContext* m_pGlobalInstance;

        QOffscreenSurface* m_pSurface;
        QOpenGLContext* m_pContext;
        bool m_bCreatedSuccessfully;
    };
}

#endif // OPENGLBACKGROUNDCONTEXT_H
