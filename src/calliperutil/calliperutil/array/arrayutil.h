#ifndef ARRAYUTIL_H
#define ARRAYUTIL_H

#include <QtGlobal>

namespace CalliperUtil
{
    template <quint32 N>
    struct TypeOfSize
    {
        typedef char type[N];
    };

    template <typename T, quint32 N>
    typename TypeOfSize<N>::type& SizeofArrayHelper(T(&)[N]);
}

#define SIZEOF_ARRAY(_arr) (sizeof(CalliperUtil::SizeofArrayHelper(_arr)))

#endif // ARRAYUTIL_H
