#ifndef STATICOBJECTSTOREKEY_H
#define STATICOBJECTSTOREKEY_H

#include <QtGlobal>

template<typename KEY, quint32 COUNT>
class StaticObjectStoreKey
{
public:
    typedef KEY KeyType;

    static quint32 count();
    static bool isValid(const KEY& key);
};

template<typename KEY, quint32 COUNT>
bool StaticObjectStoreKey<KEY, COUNT>::isValid(const KEY &key)
{
    return key >= 0 && static_cast<quint32>(key) < count();
}

template<typename KEY, quint32 COUNT>
quint32 StaticObjectStoreKey<KEY, COUNT>::count()
{
    return COUNT;
}

#endif // STATICOBJECTSTOREKEY_H
