#ifndef GLOBALINSTANCEHELPER_H
#define GLOBALINSTANCEHELPER_H

namespace CalliperUtil
{
    template<typename T>
    class GlobalInstanceHelper
    {
    public:
        template<typename... ARGS>
        static void globalInitialise(ARGS... args)
        {
            if ( m_pGlobalInstance )
            {
                return;
            }

            m_pGlobalInstance = new T(std::move(args)...);
        }

        static void globalShutdown()
        {
            if ( !m_pGlobalInstance )
            {
                return;
            }

            delete m_pGlobalInstance;
            m_pGlobalInstance = Q_NULLPTR;
        }

        static T* globalInstance()
        {
            Q_ASSERT_X(m_pGlobalInstance, Q_FUNC_INFO, "globalInstance() was called before the store was initialised!");
            return m_pGlobalInstance;
        }

    private:
        static T* m_pGlobalInstance;
    };

    template<typename T>
    T* GlobalInstanceHelper<T>::m_pGlobalInstance = Q_NULLPTR;
}

#endif // GLOBALINSTANCEHELPER_H
