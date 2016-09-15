#ifndef FIXEDINDEXPOOL_H
#define FIXEDINDEXPOOL_H

#include "calliperutil_global.h"
#include <QScopedArrayPointer>

namespace NS_CALLIPERUTIL
{
    // Can return indices ranging from 0 to size inclusive.
    // -1 is returned if no more indices are available.
    class CALLIPERUTILSHARED_EXPORT FixedIndexPool
    {
    public:
        FixedIndexPool(int size);

        int size() const;
        int occupiedIndices() const;

        int getNextIndex();
        int peekNextIndex() const;
        bool hasNextIndex() const;
        void revokeIndex(int index);
        bool isIndexOccupied(int index) const;

    private:
        const int               m_iSize;
        QScopedPointer<bool>    m_IndexList;
        int                     m_iOccupiedIndices;
    };
}

#endif // FIXEDINDEXPOOL_H
