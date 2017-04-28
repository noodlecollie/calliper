#ifndef CONTEXTREFERENCECOUNTER_H
#define CONTEXTREFERENCECOUNTER_H

#include "rendersystem_global.h"

class QOpenGLContext;
class QSurface;

namespace RenderSystem
{
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
