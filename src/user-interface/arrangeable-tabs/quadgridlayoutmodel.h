#ifndef QUADGRIDLAYOUTMODEL_H
#define QUADGRIDLAYOUTMODEL_H

#include "user-interface_global.h"
#include <QHash>
#include "quadgridlayoutinfo.h"
#include <QPoint>

class QWidget;

namespace UserInterface
{
    class QuadGridLayoutModel
    {
    public:
        enum GridCell
        {
            NoCell = 0,

            NorthEast = (1<<0),
            NorthWest = (1<<1),
            SouthEast = (1<<2),
            SouthWest = (1<<3),
        };
        Q_DECLARE_FLAGS(GridCells, GridCell)

        QuadGridLayoutModel();

        // Requires that the target cell is not occupied by a 1x1 widget.
        void addWidget(QWidget* widget, GridCell cell, Qt::Orientation preferredSplit = Qt::Horizontal);

        void removeWidget(QWidget* widget, Qt::Orientation preferredMerge = Qt::Horizontal);
        void removeWidget(GridCell cell, Qt::Orientation preferredMerge = Qt::Horizontal);
        void clear();

        QWidget* widgetAt(GridCell cell) const;

        const QuadGridLayoutInfo& layoutInfo() const;

    private:
        static int cellToArrayIndex(GridCell cell);
        static int arrayIndexToCell(int index);
        static int cellCount(const GridCells cells);
        static int pointToArrayIndex(const QPoint& p);
        static bool isArrayIndexValid(int index);
        static QPoint cellToPoint(GridCell cell);
        static GridCell pointToCell(const QPoint& p);
        static QPoint neighbour(const QPoint& p, bool horizontal);
        static GridCell cellFromFlags(const GridCells flags);

        int cellCount(QWidget* widget) const;
        GridCells widgetCells(QWidget* widget) const;
        GridCell findSingleCellNeighbour(GridCell cell, Qt::Orientation preferredDirection) const;
        int neighbourCellCount(const QPoint& point, bool horizontal) const;
        QList<GridCell> neighboursOfExtendedWidget(QWidget* widget) const;

        void setSingleWidget(QWidget* widget);
        void splitDoubleSpanWidget(QWidget* widget, GridCell cell);
        void splitSingleWidget(QWidget* widget, GridCell cell, Qt::Orientation preferredSplit);

        void mergeSingleCellNeighbour(QWidget* widget, Qt::Orientation preferredMerge);
        void mergeBothNeighbours(QWidget* widget);

        QWidget* m_IndexToWidget[4];
        QHash<QWidget*, GridCells> m_WidgetToIndex;
        QuadGridLayoutInfo m_LayoutInfo;
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(QuadGridLayoutModel::GridCells)
}

#endif // QUADGRIDLAYOUTMODEL_H
