#ifndef STATICOBJECTSTORE_H
#define STATICOBJECTSTORE_H

#include <QHash>
#include <QtDebug>

namespace Containers
{
    template<typename T, typename STOREKEY>
    class StaticObjectStore
    {
    public:
        StaticObjectStore();
        virtual ~StaticObjectStore();

        void initialise();
        void destroy();

        T object(const typename STOREKEY::KeyType& key) const;

    protected:
        typedef QHash<typename STOREKEY::KeyType, T> StaticObjectStoreHash;

        virtual void onStoreInitialised() = 0;
        virtual void onStoreDestroyed() = 0;

        StaticObjectStoreHash m_Objects;

    private:
        bool m_bStoreInitialised;
    };

    template<typename T, typename STOREKEY>
    StaticObjectStore<T, STOREKEY>::StaticObjectStore()
        : m_Objects(),
          m_bStoreInitialised(false)
    {
    }

    template<typename T, typename STOREKEY>
    StaticObjectStore<T, STOREKEY>::~StaticObjectStore()
    {
    }

    template<typename T, typename STOREKEY>
    void StaticObjectStore<T, STOREKEY>::initialise()
    {
        if ( m_bStoreInitialised )
        {
            return;
        }

        onStoreInitialised();
        const quint32 count = static_cast<quint32>(m_Objects.count());

        Q_ASSERT_X(count == STOREKEY::count(), Q_FUNC_INFO, "Initialised object count did not match expected count!");
        if ( count != STOREKEY::count() )
        {
            qFatal("%s: Expected static object count of %d but got %d.", Q_FUNC_INFO, STOREKEY::count(), m_Objects.count());
        }

        m_bStoreInitialised = true;
    }

    template<typename T, typename STOREKEY>
    void StaticObjectStore<T, STOREKEY>::destroy()
    {
        if ( !m_bStoreInitialised )
        {
            return;
        }

        onStoreDestroyed();
        m_bStoreInitialised = false;
    }

    template<typename T, typename STOREKEY>
    T StaticObjectStore<T, STOREKEY>::object(const typename STOREKEY::KeyType &key) const
    {
        Q_ASSERT_X(m_Objects.contains(key), Q_FUNC_INFO, "Object not found for key!");

        if ( !STOREKEY::isValid(key) )
        {
            return T();
        }

        return m_Objects.value(key);
    }
}

#endif // STATICOBJECTSTORE_H
