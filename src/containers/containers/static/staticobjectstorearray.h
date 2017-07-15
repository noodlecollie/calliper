#ifndef STATICOBJECTSTOREARRAY_H
#define STATICOBJECTSTOREARRAY_H

namespace Containers
{
    /*
     * Initialise like so:
     * typedef StaticObjectStoreArray<..args here..> Store;
     * const Store::InitialiserFunction initialiserFuncs[] =
     * {
     *      firstfunc,
     *      secondfunc,
     *      thirdfunc,
     *      ...
     * };
     *
     * Store store(initialiserFuncs);
     *
     * Alternatively, if you want to explicitly say which item should go
     * in which array index without relying on the ordering of the
     * initialisers, do this:
     *
     * const Store::Initialiser initialisers[] =
     * {
     *      { firstIndex, firstfunc },
     *      { secondIndex, secondfunc },
     *      { thirdIndex, thirdfunc },
     *      ...
     * };
     *
     * Store store(initialisers);
     */
    template<typename T, quint32 COUNT>
    class StaticObjectStoreArray
    {
    public:
        typedef std::function<T()> InitialiserFunction;

        struct Initialiser
        {
            const quint32 key;
            const InitialiserFunction initFunc;

            Initialiser(quint32 inKey, InitialiserFunction inInitFunc = [](){ return T(); })
                : key(inKey),
                  initFunc(inInitFunc)
            {
            }
        };

        template<quint32 N>
        inline StaticObjectStoreArray(const InitialiserFunction(&array)[N])
        {
            static_assert(N == COUNT, "Size of initialiser array does not match expected number of objects.");

            for ( quint32 itemIndex = 0; itemIndex < N; ++itemIndex )
            {
                m_ObjectArray[itemIndex] = array[itemIndex]();
            }
        }

        template<quint32 N>
        inline StaticObjectStoreArray(const Initialiser(&array)[N])
        {
            static_assert(N == COUNT, "Size of initialiser array does not match expected number of objects.");

            for ( quint32 itemIndex = 0; itemIndex < N; ++itemIndex )
            {
                const Initialiser& item = array[itemIndex];

                Q_ASSERT_X(item.key >= 0 && item.key < COUNT, Q_FUNC_INFO, "Item key outside of array bounds!");
                if ( item.key < 0 || item.key >= COUNT )
                {
                    continue;
                }

                m_ObjectArray[item.key] = item.initFunc();
            }
        }

        inline T object(quint32 index) const
        {
            return m_ObjectArray[index];
        }

        inline T& operator [](quint32 index)
        {
            return m_ObjectArray[index];
        }

    private:
        T m_ObjectArray[COUNT];
    };
}

#endif // STATICOBJECTSTOREARRAY_H
