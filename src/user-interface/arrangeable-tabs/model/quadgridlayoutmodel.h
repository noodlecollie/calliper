#ifndef QUADGRIDLAYOUTMODEL_H
#define QUADGRIDLAYOUTMODEL_H

#include "user-interface_global.h"
#include <QObject>
#include "quadgridlayoutdefs.h"
#include <QHash>
#include "quadgridlayoutpoint.h"

class QWidget;

namespace UserInterface
{
    class QuadGridLayoutModel : public QObject
    {
        Q_OBJECT
    public:
        explicit QuadGridLayoutModel(QObject *parent = nullptr);

        QWidget* widgetAt(QuadGridLayoutDefs::GridCell cell) const;

        void clear();
        bool addWidget(QWidget* widget, QuadGridLayoutDefs::GridCell cell, Qt::Orientation preferredSplit = Qt::Horizontal);
        bool removeWidget(QuadGridLayoutDefs::GridCell cell, Qt::Orientation preferredMerge = Qt::Horizontal);

    signals:
        void layoutUpdated();

    public slots:

    private:
        typedef QList<QuadGridLayoutDefs::GridCell> GridCellList;
        typedef QHash<QWidget*, GridCellList> WidgetCellMap;

        QWidget* widgetAt(QuadGridLayoutDefs::GridCell cell) const;
        QWidget* widgetAt(const QuadGridLayoutPoint& point) const;

        GridCellList widgetCells(QWidget* widget) const;
        int widgetCellCount(QWidget* widget) const;
        QuadGridLayoutDefs::WidgetSpan widgetSpan(QWidget* widget) const;

        static QuadGridLayoutDefs::GridCell neighbourCell(QuadGridLayoutDefs::GridCell cell, Qt::Orientation direction);
        static QuadGridLayoutDefs::WidgetOrdering orderingForSplit(QuadGridLayoutDefs::GridCell targetCell, Qt::Orientation splitDirection);
        static QuadGridLayoutDefs::WidgetOrdering orderingForRectangleSplit(QuadGridLayoutDefs::GridCell targetCell, QuadGridLayoutDefs::WidgetSpan span);
        static Qt::Orientation neighbourOrientation(QuadGridLayoutDefs::WidgetSpan span);


        // Adding widgets //

        // Sets all cells to contain this widget. Removes references to any other widget.
        void setSingleWidget(QWidget* widget);

        // Assuming the only existing widget occupies all four cells, splits this into two and adds the new widget to half of the cells.
        // Cells are split according to splitDirection, and the new widget is added to the higher or lower half according to the ordering.
        void splitSingleWidget(QWidget* newWidget, Qt::Orientation splitDirection, QuadGridLayoutDefs::WidgetOrdering ordering);

        // Assuming rectangleWidget spans two cells, splits this widget in half and adds the new widget into one of the cells.
        // Which cell the new widget occupies is specified by the ordering.
        void splitRectangleWidget(QWidget* rectangleWidget, QWidget* newWidget, QuadGridLayoutDefs::WidgetOrdering ordering);


        // Removing widgets //

        // Given a single-cell widget to remove, finds a neighbour that occupies a single cell and merges this neighbour into the original cell.
        // The neighbour becomes a spanning widget. The preferred neighbour is given by mergeDirection - if this neighbour does not occupy
        // a single cell, the alternate neighbour is used instead.
        void mergeSingleNeighbour(QWidget* removalWidget, Qt::Orientation mergeDirection);

        // Assuming the removal widget spans two cells and the neighbours both span one cell each, merges both single-cell neighbours
        // into the space previously occupied by the removal widget.
        void mergeDoubleNeighbours(QWidget* removalWidget);


        void updateWidget(QWidget* widget, const GridCellList& cells);

        QWidget* m_CellToWidget[4];
        WidgetCellMap m_WidgetToCells;
    };
}

#endif // QUADGRIDLAYOUTMODEL_H
