#ifndef CONTEXTREFERENCECOUNTER_H
#define CONTEXTREFERENCECOUNTER_H

#include "rendersystem_global.h"

#include <QLoggingCategory>

class QOpenGLContext;
class QSurface;

namespace RenderSystem
{
    Q_DECLARE_LOGGING_CATEGORY(lcContextReferenceCounter)

    // Ensures that the RenderSystem context is kept current while
    // at least one instance of this class is alive.
    // Note that this doesn't prevent other contexts from being made current,
    // but it will throw an error if a different context is current
    // when the main one should finish being current.
    class RENDERSYSTEMSHARED_EXPORT ContextReferenceCounter
    {
    public:
        ContextReferenceCounter();
        ~ContextReferenceCounter();

        quint32 refCount() const;

    private:
        static quint32 m_nRefCount;
        static QOpenGLContext* m_pPreviouslyCurrentContext;
        static QSurface* m_pPreviouslyCurrentSurface;
        static QOpenGLContext* m_pOwnContext;
        static bool m_bMadeCurrent;
    };
}

#endif // CONTEXTREFERENCECOUNTER_H
