#ifndef STATICOBJECTSTOREASSOCIATIVE_H
#define STATICOBJECTSTOREASSOCIATIVE_H

namespace Containers
{
    /*
     * Initialise like so:
     * typedef StaticObjectStoreAssociative<..args here..> Store;
     * const Store::Initialiser initialisers[] =
     * {
     *      { firstkey, firstfunc },
     *      { secondkey, secondfunc },
     *      { thirdkey, thirdfunc },
     *      ...
     * };
     *
     * Store store(initialisers);
     */
    template<typename KEY, typename T, quint32 COUNT = 0>
    class StaticObjectStoreAssociative
    {
    public:
        struct Initialiser
        {
            const KEY key;
            const std::function<T()> initFunc;

            Initialiser(const KEY& inKey, std::function<T()> inInitFunc = [](){ return T(); })
                : key(inKey),
                  initFunc(inInitFunc)
            {
            }
        };

        template<quint32 N>
        inline StaticObjectStoreAssociative(const Initialiser(&array)[N])
        {
            static_assert(COUNT == 0 || N == COUNT, "Size of initialiser array does not match expected number of objects.");

            for ( quint32 itemIndex = 0; itemIndex < N; ++itemIndex )
            {
                const Initialiser& initialiser = array[itemIndex];

                Q_ASSERT_X(!m_ObjectHash.contains(initialiser.key), Q_FUNC_INFO, "Duplicate object in hash!");

                m_ObjectHash.insert(initialiser.key, initialiser.initFunc());
            }
        }

        inline T object(const KEY& key) const
        {
            return m_ObjectHash.value(key, T());
        }

        inline T& operator [](const KEY& key)
        {
            return m_ObjectHash[key];
        }

    private:
        QHash<KEY, T> m_ObjectHash;
    };
}

#endif // STATICOBJECTSTOREASSOCIATIVE_H
