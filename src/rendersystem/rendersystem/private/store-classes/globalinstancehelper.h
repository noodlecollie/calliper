#ifndef GLOBALINSTANCEHELPER_H
#define GLOBALINSTANCEHELPER_H

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
        return m_pGlobalInstance;
    }

private:
    static T* m_pGlobalInstance;
};

template<typename T>
T* GlobalInstanceHelper<T>::m_pGlobalInstance = Q_NULLPTR;

#endif // GLOBALINSTANCEHELPER_H
