#ifndef GLOBALINSTANCE_H
#define GLOBALINSTANCE_H

#include "calliperutil_global.h"

namespace CalliperUtil
{
    template<typename T>
    class GlobalInstance
    {
    public:
        GlobalInstance()
        {
            if ( m_nRefCount == 0 )
            {
                m_pInstance = new T();
                m_pFirstInstantiation = this;
            }

            ++m_nRefCount;
        }

        explicit GlobalInstance(T* newInstance)
        {
            Q_ASSERT_X(!isValid(), Q_FUNC_INFO, "An instance of this class already exists!");

            if ( m_nRefCount == 0 )
            {
                m_pInstance = newInstance;
                m_pFirstInstantiation = this;
            }

            ++m_nRefCount;
        }

        ~GlobalInstance()
        {
            --m_nRefCount;

            if ( m_nRefCount == 0 )
            {
                delete m_pInstance;
                m_pInstance = nullptr;
                m_pFirstInstantiation = nullptr;
            }
        }

        static inline bool isValid()
        {
            return m_pInstance != nullptr;
        }

        static inline T* instance()
        {
            return m_pInstance;
        }

        static inline GlobalInstance<T>* firstInstantiation()
        {
            return m_pFirstInstantiation;
        }

        inline T& operator *()
        {
            return *instance();
        }

        inline const T& operator *() const
        {
            return *instance();
        }

        inline T* operator ->()
        {
            return instance();
        }

        inline const T* operator ->() const
        {
            return instance();
        }

    private:
        static T* m_pInstance;
        static quint32 m_nRefCount;
        static GlobalInstance<T>* m_pFirstInstantiation;
    };

    template<typename T>
    T* GlobalInstance<T>::m_pInstance = nullptr;

    template<typename T>
    quint32 GlobalInstance<T>::m_nRefCount = 0;

    template<typename T>
    GlobalInstance<T>* GlobalInstance<T>::m_pFirstInstantiation = nullptr;
}

#endif // GLOBALINSTANCE_H
