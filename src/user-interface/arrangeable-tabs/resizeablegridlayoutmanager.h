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
                              Qt::Orientation splitPreference);
        QWidget* insertWidget(ContentCellFlag cell, QWidget* widget);
    private slots:
        void resizeButtonDragged(int deltaX, int deltaY);

    private:
        enum SplitIndexChoice
        {
            LowerIndex = 0,
            UpperIndex,
        };

        class ResizeHandleLayout
        {
        public:
            enum LayoutType
            {
                None,
                Bar,
                Tri,
                Quad,
            };

            LayoutType type;

            Qt::Orientation orientation;
            QPoint tJunction;

            ResizeHandleLayout() : type(None), orientation(Qt::Horizontal), tJunction() {}
        };

        static bool isSingleFlag(int cell);
        static QPoint cellToIndex(ContentCellFlag cell);
        static QPoint flagToIndex(const ContentCellFlags& flags);
        static int cellCount(const ContentCellFlags& flags);
        static SplitIndexChoice indexChoice(Qt::Orientation splitPreference, ContentCellFlag cell);
        static QList<ContentCellFlag> flagList(const ContentCellFlags& flags);
        static ContentCellFlag singleFlag(const ContentCellFlags& flags);
        static QPoint tJunctionIndex(ContentCellFlag singleCell1, ContentCellFlag singleCell2);

        void initialiseGridLayout();
        void resetCellToWidgetMap();
        bool isSingleCellWidget(QWidget* widget) const;
        QWidget* getWidget(ContentCellFlag flag) const;
        ContentCellFlags getFlags(QWidget* widget) const;
        void swapWidgetFlags(QWidget* existing, QWidget* widget);
        void setHalfSplitStretch(Qt::Orientation splitDir);
        void removeWidget(QWidget* widget);
        Qt::Orientation autoSplitPreference(ContentCellFlag newCell);

        void removeResizeButtons();
        void addResizeButton(int row, int column, int rowSpan, int colSpan);
        void addVerticalResizeButton();
        void addHorizontalResizeButton();
        void rebuildResizeButtons();
        void rebuildQuadResizeButtons();
        void rebuildTResizeButtons();
        void rebuildHalfResizeButtons();
        void setHandleLayout(ResizeHandleLayout::LayoutType type);

        void splitWidget(QWidget* existing, QWidget* widget, ContentCellFlag cell, Qt::Orientation splitPreference);
        void transition1to2widgets(QWidget* existing, QWidget* widget,
                                   Qt::Orientation split, SplitIndexChoice newWidgetIndex);
        void transitionSplitRectWidget(QWidget* existing, QWidget* widget, ContentCellFlag cell);
        void setSingleWidget(QWidget* widget);

        QGridLayout* const m_pGridLayout;
        QHash<ContentCellFlag, QWidget*> m_CellToWidget;
        QHash<QWidget*, ContentCellFlags> m_WidgetToCell;
        QList<ResizeableGridElementButton*> m_ResizeButtons;
        ContentCellFlag m_iLastAddedCell;
        ResizeHandleLayout m_HandleLayout;
    };
}

#endif // RESIZEABLEGRIDLAYOUTMANAGER_H
