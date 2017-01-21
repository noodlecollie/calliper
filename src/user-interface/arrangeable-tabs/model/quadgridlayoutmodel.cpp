#include "quadgridlayoutmodel.h"
#include <QWidget>

namespace UserInterface
{
    QuadGridLayoutModel::QuadGridLayoutModel(QObject *parent) : QObject(parent)
    {
        clear();
    }

    void QuadGridLayoutModel::clear()
    {
        memset(m_CellToWidget, 0, 4 * sizeof(QWidget*));
        m_WidgetToCells.clear();
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

    QuadGridLayoutDefs::GridCell QuadGridLayoutModel::neighbourCell(QuadGridLayoutDefs::GridCell cell, Qt::Orientation direction)
    {
        return QuadGridLayoutPoint(cell).neighbour(direction).toCell();
    }

    QuadGridLayoutDefs::WidgetSpan QuadGridLayoutModel::widgetSpan(QWidget *widget) const
    {
        GridCellList cells = widgetCells(widget);

        if ( cells.count() != 2 )
            return QuadGridLayoutDefs::NoSpan;

        QuadGridLayoutDefs::GridCell cell1 = cells.at(0);
        QuadGridLayoutDefs::GridCell cell2 = cells.at(1);

        // Opposite corners - should never happen.
        if ( cell1 + cell2 = 3 )
            return QuadGridLayoutDefs::NoSpan;

        // Cells that are adjacent on X will have different values % 2.
        if ( (cell1 % 2) + (cell2 % 2) == 1 )
            return QuadGridLayoutDefs::HorizontalSpan;

        // Cells that are adjacent on Y will have a value equal to 0 after % 2.
        return QuadGridLayoutDefs::VerticalSpan;
    }
}
