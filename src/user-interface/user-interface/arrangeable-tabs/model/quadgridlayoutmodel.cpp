#include "quadgridlayoutmodel.h"
#include <QWidget>

namespace UserInterface
{
    QuadGridLayoutModel::QuadGridLayoutModel(QObject *parent)
        : QObject(parent)
    {
        clear();
    }

    void QuadGridLayoutModel::clear()
    {
        memset(m_CellToWidget, 0, 4 * sizeof(QWidget*));
        m_WidgetToCells.clear();
    }

    void QuadGridLayoutModel::deleteAllWidgets()
    {
        for ( int i = 0; i < 4; ++i )
        {
            delete m_CellToWidget[i];
        }

        clear();
    }

    bool QuadGridLayoutModel::canAddWidget(QuadGridLayoutDefs::GridCell cell) const
    {
        QWidget* existingWidget = widgetAt(cell);
        return !existingWidget || widgetCellCount(existingWidget) > 1;
    }

    bool QuadGridLayoutModel::canRemoveWidget(QuadGridLayoutDefs::GridCell cell) const
    {
        return widgetAt(cell) != nullptr;
    }

    bool QuadGridLayoutModel::canReplaceWidget(QuadGridLayoutDefs::GridCell cell) const
    {
        return widgetAt(cell) != nullptr;
    }

    bool QuadGridLayoutModel::addWidget(QWidget *widget, QuadGridLayoutDefs::GridCell cell, Qt::Orientation preferredSplit)
    {
        if ( !canAddWidget(cell) )
            return false;

        QWidget* existingWidget = widgetAt(cell);
        if ( !existingWidget )
        {
            Q_ASSERT(m_WidgetToCells.count() == 0);
            setSingleWidget(widget);
            return true;
        }

        switch ( widgetCellCount(existingWidget) )
        {
            case 1:
            {
                Q_ASSERT(m_WidgetToCells.count() == 4);

                // Don't perform operations that would completely overwrite existing widgets.
                return false;
            }

            case 2:
            {
                Q_ASSERT(m_WidgetToCells.count() == 2 || m_WidgetToCells.count() == 3);

                // Split the rectangular widget in half.
                splitRectangleWidget(existingWidget, widget, orderingForRectangleSplit(cell, widgetSpan(existingWidget)));
                break;
            }

            case 4:
            {
                Q_ASSERT(m_WidgetToCells.count() == 1);

                // We're just splitting a single widget.
                splitSingleWidget(widget, preferredSplit, orderingForSplit(cell, preferredSplit));
                break;
            }

            default:
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Unexpected number of occupied cells for existing widget!");
                return false;
            }
        }

        emit layoutUpdated();
        return true;
    }

    bool QuadGridLayoutModel::removeWidget(QuadGridLayoutDefs::GridCell cell, Qt::Orientation preferredMerge)
    {
        if ( !canRemoveWidget(cell) )
            return false;

        QWidget* existingWidget = widgetAt(cell);
        if ( !existingWidget )
        {
            Q_ASSERT(m_WidgetToCells.count() == 0);
            return false;
        }

        switch ( widgetCellCount(existingWidget) )
        {
            case 1:
            {
                Q_ASSERT(m_WidgetToCells.count() == 4 || m_WidgetToCells.count() == 3);

                mergeSingleNeighbour(existingWidget, preferredMerge);
                break;
            }

            case 2:
            {
                if ( m_WidgetToCells.count() == 3 )
                {
                    mergeDoubleNeighbours(existingWidget);
                    break;
                }
                else if ( m_WidgetToCells.count() == 2 )
                {
                    QuadGridLayoutDefs::WidgetSpan span = widgetSpan(existingWidget);
                    Qt::Orientation neighbourDir = neighbourOrientation(span);
                    setSingleWidget(widgetAt(QuadGridLayoutPoint(cell).neighbour(neighbourDir)));
                    break;
                }
                else
                {
                    Q_ASSERT(false);
                    return false;
                }
            }

            case 4:
            {
                Q_ASSERT(m_WidgetToCells.count() == 1);

                clear();
                break;
            }

            default:
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Unexpected number of occupied cells for existing widget!");
                return false;
            }
        }

        emit layoutUpdated();
        return true;
    }

    bool QuadGridLayoutModel::replaceWidget(QWidget *widget, QuadGridLayoutDefs::GridCell cell)
    {
        if ( !canReplaceWidget(cell) )
            return false;

        QWidget* currentWidget = widgetAt(cell);
        updateWidget(widget, widgetCells(currentWidget));

        emit layoutUpdated();
        return true;
    }

    QWidget* QuadGridLayoutModel::widgetAt(QuadGridLayoutDefs::GridCell cell) const
    {
        return widgetAt(QuadGridLayoutPoint(cell));
    }

    QWidget* QuadGridLayoutModel::widgetAt(const QuadGridLayoutPoint &point) const
    {
        return m_CellToWidget[point.toArrayIndex()];
    }

    QuadGridLayoutModel::GridCellList QuadGridLayoutModel::widgetCells(QWidget *widget) const
    {
        return m_WidgetToCells.value(widget, GridCellList());
    }

    int QuadGridLayoutModel::widgetCellCount(QWidget *widget) const
    {
        return widgetCells(widget).count();
    }

    int QuadGridLayoutModel::widgetCount() const
    {
        return m_WidgetToCells.count();
    }

    QList<QWidget*> QuadGridLayoutModel::widgets() const
    {
        return m_WidgetToCells.keys();
    }

    QuadGridLayoutDefs::GridCell QuadGridLayoutModel::neighbourCell(QuadGridLayoutDefs::GridCell cell, Qt::Orientation direction)
    {
        return QuadGridLayoutPoint(cell).neighbour(direction).toCell();
    }

    QuadGridLayoutDefs::WidgetOrdering QuadGridLayoutModel::orderingForSplit(QuadGridLayoutDefs::GridCell targetCell, Qt::Orientation splitDirection)
    {
        QuadGridLayoutPoint targetPoint(targetCell);
        int axisPos = splitDirection == Qt::Horizontal ? targetPoint.y() : targetPoint.x();

        return axisPos == 1 ? QuadGridLayoutDefs::NewWidgetInUpperIndex : QuadGridLayoutDefs::NewWidgetInLowerIndex;
    }

    QuadGridLayoutDefs::WidgetOrdering QuadGridLayoutModel::orderingForRectangleSplit(QuadGridLayoutDefs::GridCell targetCell,
                                                                                      QuadGridLayoutDefs::WidgetSpan span)
    {
        Q_ASSERT(span != QuadGridLayoutDefs::NoSpan);

        QuadGridLayoutPoint targetPoint(targetCell);
        int axisPos = span == QuadGridLayoutDefs::VerticalSpan ? targetPoint.y() : targetPoint.x();

        return axisPos == 1 ? QuadGridLayoutDefs::NewWidgetInUpperIndex : QuadGridLayoutDefs::NewWidgetInLowerIndex;
    }

    Qt::Orientation QuadGridLayoutModel::neighbourOrientation(QuadGridLayoutDefs::WidgetSpan span)
    {
        Q_ASSERT(span != QuadGridLayoutDefs::NoSpan);

        return span == QuadGridLayoutDefs::HorizontalSpan ? Qt::Vertical : Qt::Horizontal;
    }

    QuadGridLayoutDefs::WidgetSpan QuadGridLayoutModel::widgetSpan(QWidget *widget) const
    {
        GridCellList cells = widgetCells(widget);

        if ( cells.count() != 2 )
            return QuadGridLayoutDefs::NoSpan;

        QuadGridLayoutDefs::GridCell cell1 = cells.at(0);
        QuadGridLayoutDefs::GridCell cell2 = cells.at(1);

        // Opposite corners - should never happen.
        if ( cell1 + cell2 == 3 )
            return QuadGridLayoutDefs::NoSpan;

        // Cells that are adjacent on X will have different values % 2.
        if ( (cell1 % 2) + (cell2 % 2) == 1 )
            return QuadGridLayoutDefs::HorizontalSpan;

        // Cells that are adjacent on Y will have a value equal to 0 after % 2.
        return QuadGridLayoutDefs::VerticalSpan;
    }

    void QuadGridLayoutModel::updateWidget(QWidget *widget, const GridCellList &cells)
    {
        foreach ( QuadGridLayoutDefs::GridCell cell, cells )
        {
            // Remove this cell from the widget that currently references it.
            int cellArrayIndex = QuadGridLayoutPoint(cell).toArrayIndex();
            QWidget* w = m_CellToWidget[cellArrayIndex];

            if ( w )
            {
                GridCellList& wList = m_WidgetToCells[w];
                wList.removeOne(cell);

                if ( wList.isEmpty() )
                {
                    m_WidgetToCells.remove(w);
                }
            }

            // Replace the widget at this cell with our new widget.
            m_CellToWidget[cellArrayIndex] = widget;
        }

        // Add the widget to the map.
        m_WidgetToCells.insert(widget, cells);
    }

    void QuadGridLayoutModel::setSingleWidget(QWidget *widget)
    {
        updateWidget(widget, GridCellList()
                     << QuadGridLayoutDefs::NorthWest
                     << QuadGridLayoutDefs::NorthEast
                     << QuadGridLayoutDefs::SouthWest
                     << QuadGridLayoutDefs::SouthEast);
    }

    void QuadGridLayoutModel::splitSingleWidget(QWidget *newWidget, Qt::Orientation splitDirection, QuadGridLayoutDefs::WidgetOrdering ordering)
    {
        GridCellList newWidgetCells;

        if ( ordering == QuadGridLayoutDefs::NewWidgetInUpperIndex )
        {
            newWidgetCells.append(QuadGridLayoutDefs::SouthEast);

            if ( splitDirection == Qt::Horizontal )
            {
                newWidgetCells.append(QuadGridLayoutDefs::SouthWest);
            }
            else
            {
                newWidgetCells.append(QuadGridLayoutDefs::NorthEast);
            }
        }
        else
        {
            newWidgetCells.append(QuadGridLayoutDefs::NorthWest);

            if ( splitDirection == Qt::Horizontal )
            {
                newWidgetCells.append(QuadGridLayoutDefs::NorthEast);
            }
            else
            {
                newWidgetCells.append(QuadGridLayoutDefs::SouthWest);
            }
        }

        updateWidget(newWidget, newWidgetCells);
    }

    void QuadGridLayoutModel::splitRectangleWidget(QWidget *rectangleWidget, QWidget *newWidget, QuadGridLayoutDefs::WidgetOrdering ordering)
    {
        Q_ASSERT(widgetSpan(rectangleWidget) != QuadGridLayoutDefs::NoSpan);

        GridCellList cellList = widgetCells(rectangleWidget);
        Q_ASSERT(cellList.count() == 2);

        int lowerIndex = cellList.at(0) < cellList.at(1) ? 0 : 1;
        int upperIndex = lowerIndex == 0 ? 1 : 0;

        if ( ordering == QuadGridLayoutDefs::NewWidgetInLowerIndex )
        {
            cellList.removeAt(upperIndex);
        }
        else
        {
            cellList.removeAt(lowerIndex);
        }

        updateWidget(newWidget, cellList);
    }

    void QuadGridLayoutModel::mergeSingleNeighbour(QWidget *removalWidget, Qt::Orientation mergeDirection)
    {
        GridCellList removalCellList = widgetCells(removalWidget);
        Q_ASSERT(removalCellList.count() == 1);

        QuadGridLayoutDefs::GridCell removalCell = removalCellList.at(0);
        QuadGridLayoutDefs::GridCell targetCell = neighbourCell(removalCell, mergeDirection);
        QWidget* targetWidget = widgetAt(targetCell);

        if ( widgetCellCount(targetWidget) != 1 )
        {
            if ( mergeDirection == Qt::Horizontal )
            {
                mergeDirection = Qt::Vertical;
            }
            else
            {
                mergeDirection = Qt::Horizontal;
            }

            targetCell = QuadGridLayoutPoint(removalCell).neighbour(mergeDirection).toCell();
            targetWidget = widgetAt(targetCell);

            Q_ASSERT(widgetCellCount(targetWidget) == 1);
        }

        updateWidget(targetWidget, widgetCells(targetWidget) << removalCell);
    }

    void QuadGridLayoutModel::mergeDoubleNeighbours(QWidget *removalWidget)
    {
        QuadGridLayoutDefs::WidgetSpan span = widgetSpan(removalWidget);
        Q_ASSERT(span != QuadGridLayoutDefs::NoSpan);

        Qt::Orientation mergeDirection =
                span == QuadGridLayoutDefs::HorizontalSpan
                    ? Qt::Vertical
                    : Qt::Horizontal;

        GridCellList cellList = widgetCells(removalWidget);
        Q_ASSERT(cellList.count() == 2);

        QuadGridLayoutDefs::GridCell neighbourCell0 = neighbourCell(cellList.at(0), mergeDirection);
        QuadGridLayoutDefs::GridCell neighbourCell1 = neighbourCell(cellList.at(1), mergeDirection);

        Q_ASSERT(widgetAt(neighbourCell0) != widgetAt(neighbourCell1));

        updateWidget(widgetAt(neighbourCell0), GridCellList() << neighbourCell0 << cellList.at(0));
        updateWidget(widgetAt(neighbourCell1), GridCellList() << neighbourCell1 << cellList.at(1));
    }

    QuadGridLayoutDefs::GridCell QuadGridLayoutModel::lowestGridCell(const GridCellList &list)
    {
        Q_ASSERT(!list.isEmpty());
        if ( list.isEmpty() )
            return QuadGridLayoutDefs::NorthWest;

        QuadGridLayoutDefs::GridCell lowestCell = QuadGridLayoutDefs::SouthEast;
        foreach ( QuadGridLayoutDefs::GridCell cell, list )
        {
            if ( cell < lowestCell )
            {
                lowestCell = cell;
            }
        }

        return lowestCell;
    }

    bool QuadGridLayoutModel::containsWidget(QWidget* widget) const
    {
        return m_WidgetToCells.contains(widget);
    }
}
