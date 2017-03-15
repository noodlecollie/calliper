#ifndef SCOPEDCURRENTCONTEXT_H
#define SCOPEDCURRENTCONTEXT_H

#include "renderer_global.h"

class QOpenGLContext;
class QSurface;

namespace Renderer
{
    // Makes the given context current on construction, and calls doneCurrent()
    // on the context on destruction. If no context is provided, the background
    // context is used.
    // WARNING: It's best not to mix and match these! Unless you're re-calling
    // with the background context, this class will quit (or fail assertions in
    // debug mode) if there is already another context that has been made current
    // with a ScopedCurrentContext. Regardless, it's highly recommended not to
    // use any ScopedCurrentContexts or make any other contexts current while one
    // ScopedCurrentContext already exists.
    class RENDERERSHARED_EXPORT ScopedCurrentContext
    {
    public:
        ScopedCurrentContext();
        ScopedCurrentContext(QOpenGLContext* context, QSurface* surface);
        ~ScopedCurrentContext();

        bool wasSuccessful() const;

    private:
        static QOpenGLContext* m_pContext;

        bool m_bInvalid;
        bool m_bShouldCleanUp;
    };
}

#endif // SCOPEDCURRENTCONTEXT_H
