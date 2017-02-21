#ifndef OPENGLBACKGROUNDCONTEXT_H
#define OPENGLBACKGROUNDCONTEXT_H

#include "renderer_global.h"
#include <QOpenGLContext>
#include <QOffscreenSurface>

namespace Renderer
{
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

        QOpenGLContext* m_pContext;
        QOffscreenSurface* m_pSurface;
        bool m_bCreatedSuccessfully;
    };
}

#endif // OPENGLBACKGROUNDCONTEXT_H
