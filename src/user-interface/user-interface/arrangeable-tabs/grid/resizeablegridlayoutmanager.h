#ifndef RESIZEABLEGRIDLAYOUTMANAGER_H
#define RESIZEABLEGRIDLAYOUTMANAGER_H

#include "user-interface_global.h"
#include <QObject>
#include <QPoint>
#include <QPointer>
#include <QHash>
#include "user-interface/arrangeable-tabs/model/quadgridlayoutdefs.h"
#include "user-interface/arrangeable-tabs/model/quadgridlayoutmodel.h"
#include "user-interface/arrangeable-tabs/model/quadgridlayoutanalyser.h"

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

        // Inserts a widget and splits existing containers if possible.
        // If it's not possible to split, inserts the widget into the container.
        void addWidget(QWidget* widget, QuadGridLayoutDefs::GridCell cell, Qt::Orientation splitPreference);

        // Inserts a widget directly into the container located in the specified cell.
        void insertWidget(QWidget* widget, QuadGridLayoutDefs::GridCell cell);

        QWidget* takeActiveWidget(QuadGridLayoutDefs::GridCell cell, Qt::Orientation mergePreference);

        void equaliseCellSizes();

    signals:
        void widgetFloated(QWidget* widget, const QPoint& globalPos, bool dragged);

    private slots:
        void resizeButtonDragged(int deltaX, int deltaY);
        void maximizeInvoked(int itemId);
        void closeInvoked(int itemId);
        void floatInvoked(int itemId, bool dragged);

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

        void updateStretchFactors();
        void setSingleWidgetStretchFactors();
        void setDualWidgetStretchFactors();
        void setTripleWidgetStretchFactors();
        void setStretchFactors(int row0, int row2, int col0, int col2);

        void resizeHorizontal(int delta);
        void resizeVertical(int delta);
        void maximize(QuadGridLayoutDefs::GridCell cell);
        QWidget* removeWidget(ResizeableGridLayoutContainer* container, int index, Qt::Orientation mergePreference);

        ResizeableGridLayoutContainer* containerAt(QuadGridLayoutDefs::GridCell cell) const;
        ResizeableGridLayoutContainer* createContainerForEmbed(QuadGridLayoutDefs::GridCell cell, Qt::Orientation splitPreference);
        ResizeableGridLayoutContainer* createContainerForInsert(QuadGridLayoutDefs::GridCell cell);
        ResizeableGridLayoutContainer* createContainer();

        QGridLayout* const m_pGridLayout;
        QList<ResizeableGridElementButton*> m_ResizeButtons;

        QuadGridLayoutModel* m_pModel;
        QuadGridLayoutAnalyser* m_pAnalyser;
    };
}

#endif // RESIZEABLEGRIDLAYOUTMANAGER_H
