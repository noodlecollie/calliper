#ifndef STATICOBJECTSTORE_H
#define STATICOBJECTSTORE_H

#include <QHash>
#include <QtDebug>

template<typename T, typename KEY, quint32 COUNT>
class StaticObjectStore
{
public:
    StaticObjectStore();
    virtual ~StaticObjectStore();

    void initialise();
    void destroy();

    T object(const KEY& key) const;

protected:
    typedef QHash<KEY, T> StaticObjectStoreHash;

    virtual void storeInitialised() = 0;
    virtual void storeDestroyed() = 0;

    QHash<KEY, T> m_Objects;

private:
    bool m_bStoreInitialised;
};

template<typename T, typename KEY, quint32 COUNT>
StaticObjectStore<T, KEY, COUNT>::StaticObjectStore()
    : m_Objects(),
      m_bStoreInitialised(false)
{
}

template<typename T, typename KEY, quint32 COUNT>
StaticObjectStore<T, KEY, COUNT>::~StaticObjectStore()
{
    destroy();
}

template<typename T, typename KEY, quint32 COUNT>
void StaticObjectStore<T, KEY, COUNT>::initialise()
{
    if ( m_bStoreInitialised )
    {
        return;
    }

    storeInitialised();

    Q_ASSERT_X(m_Objects.count() == COUNT, Q_FUNC_INFO, "Initialised object count did not match expected count!");
    if ( m_Objects.count() != COUNT )
    {
        qFatal("%s: Expected static object count of %d but got %d.", Q_FUNC_INFO, COUNT, m_Objects.count());
    }

    m_bStoreInitialised = true;
}

template<typename T, typename KEY, quint32 COUNT>
void StaticObjectStore<T, KEY, COUNT>::destroy()
{
    if ( !m_bStoreInitialised )
    {
        return;
    }

    storeDestroyed();
    m_bStoreInitialised = false;
}

template<typename T, typename KEY, quint32 COUNT>
T StaticObjectStore<T, KEY, COUNT>::object(const KEY &key) const
{
    Q_ASSERT_X(m_Objects.contains(key), Q_FUNC_INFO, "Object not found for key!");

    return m_Objects.value(key);
}

#endif // STATICOBJECTSTORE_H
