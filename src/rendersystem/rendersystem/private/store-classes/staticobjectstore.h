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

    T& object(const KEY& key);
    const T& object(const KEY& key) const;

protected:
    virtual void storeInitialised() = 0;
    virtual void storeDestroyed() = 0;

    QHash<KEY, T> m_Objects;
};

template<typename T, typename KEY, quint32 COUNT>
StaticObjectStore<T, KEY, COUNT>::StaticObjectStore()
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
    storeInitialised();

    Q_ASSERT_X(m_Objects.count() == COUNT, Q_FUNC_INFO, "Initialised object count did not match expected count!");
}

template<typename T, typename KEY, quint32 COUNT>
void StaticObjectStore<T, KEY, COUNT>::destroy()
{
    storeDestroyed();
}

template<typename T, typename KEY, quint32 COUNT>
T& StaticObjectStore<T, KEY, COUNT>::object(const KEY &key)
{
    Q_ASSERT_X(m_Objects.contains(key), Q_FUNC_INFO, "Object not found for key!");

    return m_Objects.value(key);
}

template<typename T, typename KEY, quint32 COUNT>
const T& StaticObjectStore<T, KEY, COUNT>::object(const KEY &key) const
{
    Q_ASSERT_X(m_Objects.contains(key), Q_FUNC_INFO, "Object not found for key!");

    return m_Objects.value(key);
}

#endif // STATICOBJECTSTORE_H
