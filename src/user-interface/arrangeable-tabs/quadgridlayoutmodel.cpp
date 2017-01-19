#include "quadgridlayoutmodel.h"
#include <QWidget>

namespace UserInterface
{
    QuadGridLayoutModel::QuadGridLayoutModel()
    {
        clear();
    }

    void QuadGridLayoutModel::addWidget(QWidget *widget, GridCell cell, Qt::Orientation preferredSplit)
    {
        if ( cell == NoCell || widget == nullptr )
            return;

        QWidget* existing = widgetAt(cell);
        if ( !existing )
        {
            setSingleWidget(widget);
            return;
        }

        int occupiedCells = cellCount(existing);
        switch ( occupiedCells )
        {
            case 1:
            {
                // Cannot subdivide existing widget any further.
                break;
            }

            case 2:
            {
                splitDoubleSpanWidget(widget, cell);
                break;
            }

            case 4:
            {
                splitSingleWidget(widget, cell, preferredSplit);
                break;
            }

            default:
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Unexpected number of occupied cells!");
                break;
            }
        }
    }

    void QuadGridLayoutModel::removeWidget(GridCell cell, Qt::Orientation preferredMerge)
    {
        if ( cell == NoCell )
            return;

        removeWidget(widgetAt(cell), preferredMerge);
    }

    void QuadGridLayoutModel::removeWidget(QWidget *widget, Qt::Orientation preferredMerge)
    {
        if ( widget == nullptr )
            return;

        GridCells occupiedCells = widgetCells(widget);
        int occupiedCellCount = cellCount(occupiedCells);

        switch ( occupiedCellCount )
        {
            case 1:
            {
                mergeSingleCellNeighbour(widget, preferredMerge);
                break;
            }

            case 2:
            {
                mergeBothNeighbours(widget);
                break;
            }

            case 4:
            {
                clear();
                break;
            }

            default:
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Unexpected number of occupied cells!");
                break;
            }
        }
    }

    void QuadGridLayoutModel::mergeSingleCellNeighbour(QWidget *widget, Qt::Orientation preferredMerge)
    {
        // Remove the specified widget and merge in
        // the other neighbour that has a cell count
        // of 1.
        // How do we pick one if they both do?
        // Specify a preferred split (H or V)
        // and merge in the neighbour in the
        // specified direction - from side if H,
        // from above/below if V.

        GridCells cellFlags = widgetCells(widget);
        GridCell cell = cellFromFlags(cellFlags);
        Q_ASSERT(cell != NoCell);

        GridCell neighbour = findSingleCellNeighbour(cell, preferredMerge);
        Q_ASSERT(neighbour != NoCell);

        QWidget* neighbourWidget = widgetAt(neighbour);
        m_IndexToWidget[cellToArrayIndex(cell)] = neighbourWidget;
        m_WidgetToIndex[neighbourWidget] |= cell;
    }

    void QuadGridLayoutModel::mergeBothNeighbours(QWidget* widget)
    {
        QList<GridCell> neighbours = neighboursOfExtendedWidget(widget);
        Q_ASSERT(neighbours.count() == 2);
    }

    QList<QuadGridLayoutModel::GridCell> QuadGridLayoutModel::neighboursOfExtendedWidget(QWidget *widget) const
    {
        GridCells cells = widgetCells(widget);

        if ( cells.testFlag(NorthWest) )
        {

        }
    }

    QWidget* QuadGridLayoutModel::widgetAt(GridCell cell) const
    {
        if ( cell == NoCell )
            return nullptr;

        return m_IndexToWidget[cellToArrayIndex(cell)];
    }

    const QuadGridLayoutInfo& QuadGridLayoutModel::layoutInfo() const
    {
        return m_LayoutInfo;
    }

    void QuadGridLayoutModel::splitDoubleSpanWidget(QWidget *widget, GridCell cell)
    {
        // TODO
    }

    void QuadGridLayoutModel::splitSingleWidget(QWidget *widget, GridCell cell, Qt::Orientation preferredSplit)
    {
        // TODO
    }

    void QuadGridLayoutModel::setSingleWidget(QWidget *widget)
    {
        for ( int i = 0; i < 4; ++i )
        {
            m_IndexToWidget[i] = widget;
        }

        m_WidgetToIndex.clear();
        m_WidgetToIndex.insert(widget,
                               GridCells() | NorthEast | NorthWest | SouthEast | SouthWest);

        m_LayoutInfo.setMajorMinorSplit(QuadGridLayoutInfo::MajorNone,
                                        QuadGridLayoutInfo::MinorNone);
    }

    void QuadGridLayoutModel::clear()
    {
        memset(&m_IndexToWidget[0], 0, 4 * sizeof(QWidget*));
        m_WidgetToIndex.clear();
        m_LayoutInfo.setMajorMinorSplit(QuadGridLayoutInfo::MajorNone,
                                        QuadGridLayoutInfo::MinorNone);
    }

    int QuadGridLayoutModel::cellCount(QWidget *widget) const
    {
        if ( !widget )
            return 0;

        return cellCount(widgetCells);
    }

    QuadGridLayoutModel::GridCells QuadGridLayoutModel::widgetCells(QWidget *widget) const
    {
        if ( !widget )
            return GridCells();

        return m_WidgetToIndex.value(widget, GridCells());
    }

    int QuadGridLayoutModel::cellToArrayIndex(GridCell cell)
    {
        // Row-major order.
        switch ( cell )
        {
            case NorthWest:
                return 0;

            case NorthEast:
                return 1;

            case SouthWest:
                return 2;

            case SouthEast:
                return 3;

            default:
                return -1;
        }
    }

    int QuadGridLayoutModel::pointToArrayIndex(const QPoint &p)
    {
        return (2 * point.y()) + point.x();
    }

    bool QuadGridLayoutModel::isArrayIndexValid(int index)
    {
        return index >= 0 && index <= 3;
    }

    QPoint QuadGridLayoutModel::cellToPoint(GridCell cell)
    {
        switch ( cell )
        {
            case NorthWest:
                return QPoint(0,0);

            case NorthEast:
                return QPoint(1,0);

            case SouthWest:
                return QPoint(0,1);

            case SouthEast:
                return QPoint(1,1);

            default:
                return QPoint(-1,-1);
        }
    }

    QuadGridLayoutModel::GridCell QuadGridLayoutModel::pointToCell(const QPoint &p)
    {
        if ( p.x() == 0 )
        {
            if ( p.y() == 0 )
            {
                return NorthWest;
            }
            else if ( p.y() == 1 )
            {
                return SouthWest;
            }
        }
        else if ( p.x() == 1 )
        {
            if ( p.y() == 0 )
            {
                return NorthEast;
            }
            else if ( p.y() == 1 )
            {
                return NorthWest;
            }
        }

        return NoCell;
    }

    QuadGridLayoutModel::GridCell QuadGridLayoutModel::arrayIndexToCell(int index)
    {
        // Row-major order.
        switch ( index )
        {
            case 0:
                return NorthWest;

            case 1:
                return NorthEast;

            case 2:
                return SouthWest;

            case 3:
                return SouthEast;

            default:
                return NoCell;
        }
    }

    int QuadGridLayoutModel::cellCount(const GridCells cells)
    {
        int count = 0;

        if ( cells.testFlag(NorthEast) )
            ++count;

        if ( cells.testFlag(NorthWest) )
            ++count;

        if ( cells.testFlag(SouthEast) )
            ++count;

        if ( cells.testFlag(SouthWest) )
            ++count;

        return count;
    }

    QuadGridLayoutModel::GridCell
        QuadGridLayoutModel::findSingleCellNeighbour(GridCell cell, Qt::Orientation preferredDirection) const
    {
        Q_ASSERT(cell != NoCell);

        QPoint orig = cellToPoint(cell);
        int neighbourHCellCount = neighbourCellCount(orig, true);
        int neighbourVCellCount = neighbourCellCount(orig, false);

        if ( neighbourHCellCount != neighbourVCellCount )
        {
            if ( neighbourHCellCount == 1 )
                return pointToCell(neighbourPointH);

            if ( neighbourPointV == 1 )
                return pointToCell(neighbourPointV);
        }
        else
        {
            if ( neighbourHCellCount == 1 )
                return pointToCell(preferredDirection == Qt::Horizontal
                                   ? neighbourPointH
                                   : neighbourPointV);
        }

        return NoCell;

    }

    int QuadGridLayoutModel::neighbourCellCount(const QPoint& point, bool horizontal) const
    {
        QPoint neighbourPoint = neighbour(orig, horizontal);
        QWidget* neighbourWidget = widgetAt(pointToArrayIndex(neighbourPoint));
        return cellCount(widgetCells(neighbourWidget));
    }

    QPoint QuadGridLayoutModel::neighbour(const QPoint &p, bool horizontal)
    {
        if ( horizontal )
        {
            return QPoint(p.x() == 0 ? 1 : 0, p.y());
        }
        else
        {
            return QPoint(p.x(), p.y() == 0 ? 1 : 0);
        }
    }

    QuadGridLayoutModel::GridCell QuadGridLayoutModel::cellFromFlags(const GridCells flags)
    {
        Q_ASSERT(cellCount(flags) == 1);

        if ( flags.testFlag(NorthWest) )
            return NorthWest;

        if ( flags.testFlag(NorthEast) )
            return NorthEast;

        if ( flags.testFlag(SouthWest) )
            return SouthWest;

        if ( flags.testFlag(SouthEast) )
            return SouthEast;

        return NoCell;
    }
}
