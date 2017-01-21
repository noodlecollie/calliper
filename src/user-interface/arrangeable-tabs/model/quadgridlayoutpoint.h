#ifndef QUADGRIDLAYOUTPOINT_H
#define QUADGRIDLAYOUTPOINT_H

#include "user-interface_global.h"
#include "quadgridlayoutdefs.h"
#include <QtGlobal>

namespace UserInterface
{
    class USERINTERFACESHARED_EXPORT QuadGridLayoutPoint
    {
    public:
        QuadGridLayoutPoint(int x, int y);
        explicit QuadGridLayoutPoint(QuadGridLayoutDefs::GridCell cell);

        int x() const;
        void setX(int x);

        int y() const;
        void setY(int y);

        QuadGridLayoutDefs::GridCell toCell() const;
        int toArrayIndex() const;

        QuadGridLayoutPoint neighbour(Qt::Orientation direction) const;

    private:
        int m_iX;
        int m_iY;
    };
}

#endif // QUADGRIDLAYOUTPOINT_H
