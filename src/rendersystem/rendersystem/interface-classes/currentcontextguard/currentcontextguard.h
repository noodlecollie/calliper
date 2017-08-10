#ifndef CURRENTCONTEXTGUARD_H
#define CURRENTCONTEXTGUARD_H

#include "rendersystem_global.h"

#include <QOpenGLContext>

#include "contextreferencecounter.h"

#include "rendersystem/global/rendersystem.h"

#include "calliperutil/opengl/openglhelpers.h"
#include "calliperutil/opengl/openglerrors.h"

/*
 * When using this class, the interface provided as the type T
 * should have functions that require the render system context
 * be marked as non-const, and functions that don't require this
 * be marked as const. The const functions can then be used
 * via a raw const T*, and the non-const functions should require
 * a context guard in order to be used.
 */
namespace RenderSystem
{
    template<typename T>
    class CurrentContextGuard
    {
    public:
        CurrentContextGuard(T& functions);
        CurrentContextGuard(const CurrentContextGuard<T>& other);
        ~CurrentContextGuard();

        T* operator ->();
        const T* operator ->() const;

        T& operator *();
        const T& operator *() const;

        void finish();

    private:
        bool operator =(const CurrentContextGuard& other) = delete;

        ContextReferenceCounter m_RefCounter;
        T& m_Functions;
    };

    template<typename T>
    CurrentContextGuard<T>::CurrentContextGuard(T& functions)
        : m_RefCounter(),
          m_Functions(functions)
    {

    }

    template<typename T>
    CurrentContextGuard<T>::CurrentContextGuard(const CurrentContextGuard<T>& other)
        : m_RefCounter(),
          m_Functions(other.m_Functions)
    {

    }

    template<typename T>
    CurrentContextGuard<T>::~CurrentContextGuard()
    {

    }

    template<typename T>
    T* CurrentContextGuard<T>::operator ->()
    {
        return &m_Functions;
    }

    template<typename T>
    const T* CurrentContextGuard<T>::operator ->() const
    {
        return &m_Functions;
    }

    template<typename T>
    T& CurrentContextGuard<T>::operator *()
    {
        return m_Functions;
    }

    template<typename T>
    const T& CurrentContextGuard<T>::operator *() const
    {
        return m_Functions;
    }

    template<typename T>
    void CurrentContextGuard<T>::finish()
    {
        GL_CURRENT_F;
        GLTRY(f->glFinish());
    }
}

#endif // CURRENTCONTEXTGUARD_H
