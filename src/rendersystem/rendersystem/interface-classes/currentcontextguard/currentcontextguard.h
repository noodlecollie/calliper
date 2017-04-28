#ifndef CURRENTCONTEXTGUARD_H
#define CURRENTCONTEXTGUARD_H

#include "rendersystem_global.h"

#include <QOpenGLContext>

#include "contextreferencecounter.h"

#include "rendersystem/global/rendersystem.h"

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

    private:
        CurrentContextGuard() = delete;
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
}

#endif // CURRENTCONTEXTGUARD_H
