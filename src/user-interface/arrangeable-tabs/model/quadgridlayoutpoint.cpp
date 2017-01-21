#include "quadgridlayoutpoint.h"

namespace UserInterface
{
    QuadGridLayoutPoint::QuadGridLayoutPoint(int x, int y)
        : m_iX(0), m_iY(0)
    {
        setX(x);
        setY(y);
    }

    QuadGridLayoutPoint::QuadGridLayoutPoint(QuadGridLayoutDefs::GridCell cell)
        : m_iX(0), m_iY(0)
    {
        setX(cell % 2);
        setY(cell / 2);
    }

    int QuadGridLayoutPoint::x() const
    {
        return m_iX;
    }

    void QuadGridLayoutPoint::setX(int x)
    {
        m_iX = qBound(0, x, 1);
    }

    int QuadGridLayoutPoint::y() const
    {
        return m_iY;
    }

    void QuadGridLayoutPoint::setY(int y)
    {
        m_iY = qBound(0, y, 1);
    }

    int QuadGridLayoutPoint::toArrayIndex() const
    {
        return (2 * m_iY) + m_iX;
    }

    QuadGridLayoutPoint QuadGridLayoutPoint::neighbour(Qt::Orientation direction) const
    {
        return QuadGridLayoutPoint(direction == Qt::Horizontal ? (m_iX + 1) % 2 : m_iX,
                                   direction == Qt::Vertical   ? (m_iY + 1) % 2 : m_iY);
    }

    QuadGridLayoutDefs::GridCell QuadGridLayoutPoint::toCell() const
    {
        if ( m_iY == 0 )
        {
            return m_iX == 0 ? QuadGridLayoutDefs::NorthWest : QuadGridLayoutDefs::NorthEast;
        }
        else
        {
            return m_iX == 0 ? QuadGridLayoutDefs::SouthWest : QuadGridLayoutDefs::SouthEast;
        }
    }
}
