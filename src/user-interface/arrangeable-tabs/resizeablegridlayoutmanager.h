#ifndef RESIZEABLEGRIDLAYOUTMANAGER_H
#define RESIZEABLEGRIDLAYOUTMANAGER_H

#include "user-interface_global.h"
#include <QObject>
#include <QPoint>
#include <QHash>

class QGridLayout;
class QWidget;

namespace UserInterface
{
    class ResizeableGridElementButton;

    // Manages a 3x3 grid, where possible content cells are the corner cells.
    // Other cells are dividers used to resize the grid.
    class USERINTERFACESHARED_EXPORT ResizeableGridLayoutManager : public QObject
    {
        Q_OBJECT
    public:
        // Higher 16 bits are X, lower 16 bits are Y.
#define ENCODE_CONTENTS_CELL(x,y) ((x) << 16) | (y)
        enum ContentCell
        {
            NoCell = ~0,
            UpperLeft   = ENCODE_CONTENTS_CELL(0,0),
            UpperRight  = ENCODE_CONTENTS_CELL(2,0),
            LowerLeft   = ENCODE_CONTENTS_CELL(0,2),
            LowerRight  = ENCODE_CONTENTS_CELL(2,2),
        };
#undef ENCODE_CONTENTS_CELL

        explicit ResizeableGridLayoutManager(QGridLayout* gridLayout);
        ~ResizeableGridLayoutManager();

        void addWidget(QWidget* widget, ContentCell cell);
        QWidget* widgetAt(ContentCell cell) const;
        QWidget* takeWidget(ContentCell cell);

    private slots:
        void resizeButtonDragged(int deltaX, int deltaY);

    private:
        void updateLayout();
        void clearLayout();
        void setSingleItemLayout();
        void setDualItemLayout();
        void setTripleItemLayout();
        void setQuadItemLayout();
        void addResizeButton(int row, int column, int rowSpan = 1, int colSpan = 1);
        void resizeHorizontal(int delta);
        void resizeVertical(int delta);
        void setRowColMinSize();
        void setStretchFactors(int row0, int row2, int col0, int col2);

        // Returns true if split is vertical, false if it's horizontal.
        // Lower is the item whose co-ordinate in the item axis is lower
        // (ie. lower Y co-ord for a horizontal split, lower X co-ord for a vertical split).
        // Upper is the other item.
        bool calculateDualItemSplit(ContentCell cell1, ContentCell cell2, ContentCell& lower, ContentCell& upper);

        QGridLayout* const m_pGridLayout;
        QHash<ContentCell, QWidget*> m_OccupiedCells;
        ContentCell m_iLastCell;
        QList<ResizeableGridElementButton*> m_ResizeButtons;
    };
}

#endif // RESIZEABLEGRIDLAYOUTMANAGER_H
