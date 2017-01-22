#ifndef RESIZEABLEGRIDLAYOUTMANAGER_H
#define RESIZEABLEGRIDLAYOUTMANAGER_H

#include "user-interface_global.h"
#include <QObject>
#include <QPoint>
#include <QPointer>
#include <QHash>
#include "model/quadgridlayoutdefs.h"
#include "model/quadgridlayoutmodel.h"
#include "model/quadgridlayoutanalyser.h"

class QGridLayout;
class QWidget;

namespace UserInterface
{
    class ResizeableGridElementButton;
    class ResizeableGridLayoutContainer;

    // Manages a 3x3 grid, where possible content cells are the corner cells.
    // Other cells are dividers used to resize the grid.
    class USERINTERFACESHARED_EXPORT ResizeableGridLayoutManager : public QObject
    {
        Q_OBJECT
    public:
        explicit ResizeableGridLayoutManager(QGridLayout* gridLayout);
        ~ResizeableGridLayoutManager();

        // Returns the widget that was already in this position,
        // if there was one and it couldn't be split.
        // Otherwise returns null.
        QWidget* insertWidget(QWidget* widget, QuadGridLayoutDefs::GridCell cell,
                              Qt::Orientation splitPreference);

        // Returns the widget that was in this position, if there was one.
        // Otherwise returns null.
        QWidget* removeWidget(QuadGridLayoutDefs::GridCell cell, Qt::Orientation mergePreference);

        void equaliseCellSizes();

    private slots:
        void resizeButtonDragged(int deltaX, int deltaY);

    private:
        void updateGridLayout();
        void clearGridLayout();

        void setSingleWidgetLayout();
        void genericAddWidgets();

        void addResizeButtons();
        void addMajorSplitOnly();
        void addMajorMinorSplit();
        void addResizeButton(int row, int column, int rowSpan, int colSpan);

        void addFullSpanWidgetToLayout(QWidget* widget);
        void addWidgetToLayout(QWidget* widget, const QuadGridLayoutPoint& point, QuadGridLayoutDefs::WidgetSpan span);
        void addWidgetToLayout(QWidget* widget, int row, int col, int rowSpan, int colSpan);
        void addWidgetToLayout(QWidget* widget);
        bool cleanUpContainer(ResizeableGridLayoutContainer* container);
        bool cleanUpButton(ResizeableGridElementButton* button);

        void updateStretchFactors();
        void setSingleWidgetStretchFactors();
        void setDualWidgetStretchFactors();
        void setTripleWidgetStretchFactors();
        void setStretchFactors(int row0, int row2, int col0, int col2);

        void resizeHorizontal(int delta);
        void resizeVertical(int delta);

        QGridLayout* const m_pGridLayout;
        QList<ResizeableGridElementButton*> m_ResizeButtons;

        QuadGridLayoutModel* m_pModel;
        QuadGridLayoutAnalyser* m_pAnalyser;
    };
}

#endif // RESIZEABLEGRIDLAYOUTMANAGER_H
