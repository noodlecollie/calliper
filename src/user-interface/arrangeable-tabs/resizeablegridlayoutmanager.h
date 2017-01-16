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
        enum ContentCellFlag
        {
            NoCell = 0,
            UpperLeft = (1<<0),
            UpperRight = (1<<1),
            LowerLeft = (1<<2),
            LowerRight = (1<<3),
        };
        Q_DECLARE_FLAGS(ContentCellFlags, ContentCellFlag)

        explicit ResizeableGridLayoutManager(QGridLayout* gridLayout);
        ~ResizeableGridLayoutManager();

        // Returns the widget that was already in this position,
        // if there was one and it couldn't be split.
        // Otherwise returns null.
        QWidget* insertWidget(ContentCellFlag cell, QWidget* widget,
                              Qt::Orientation splitPreference = Qt::Horizontal);

    private:
        enum SplitIndexChoice
        {
            LowerIndex = 0,
            UpperIndex,
        };

        static bool isSingleFlag(int cell);
        static QPoint cellToIndex(ContentCellFlag cell);
        static QPoint flagToIndex(const ContentCellFlags& flags);
        static int cellCount(const ContentCellFlags& flags);
        static SplitIndexChoice indexChoice(Qt::Orientation splitPreference, ContentCellFlag cell);
        static QList<ContentCellFlag> flagList(const ContentCellFlags& flags);
        static ContentCellFlag singleFlag(const ContentCellFlags& flags);

        void initialiseGridLayout();
        void resetCellToWidgetMap();
        bool isSingleCellWidget(QWidget* widget) const;
        QWidget* getWidget(ContentCellFlag flag) const;
        ContentCellFlags getFlags(QWidget* widget) const;
        void swapWidgetFlags(QWidget* existing, QWidget* widget);
        void setFlags(QWidget* widget, int flags);

        void removeResizeButtons();
        void addResizeButton(int row, int column, int rowSpan, int colSpan);
        void addVerticalResizeButton();
        void addHorizontalResizeButton();
        void removeResizeButton(int row, int col);

        void splitWidget(QWidget* existing, QWidget* widget, ContentCellFlag cell, Qt::Orientation splitPreference);
        void transition1to2widgets(QWidget* existing, QWidget* widget,
                                   Qt::Orientation split, SplitIndexChoice newWidgetIndex);
        void transitionSplitRectWidget(QWidget* existing, QWidget* widget, ContentCellFlag cell);

        QGridLayout* const m_pGridLayout;
        QHash<ContentCellFlag, QWidget*> m_CellToWidget;
        QHash<QWidget*, ContentCellFlags> m_WidgetToCell;
    };
}

#endif // RESIZEABLEGRIDLAYOUTMANAGER_H
