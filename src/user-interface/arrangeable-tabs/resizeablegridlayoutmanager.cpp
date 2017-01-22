#include "resizeablegridlayoutmanager.h"
#include <QGridLayout>
#include <QWidget>
#include "resizeablegridlayoutcontainer.h"
#include "resizeablegridelementbutton.h"

namespace UserInterface
{
    ResizeableGridLayoutManager::ResizeableGridLayoutManager(QGridLayout* gridLayout)
        : m_pGridLayout(gridLayout),
          m_ResizeButtons(),
          m_pModel(new QuadGridLayoutModel(this)),
          m_pAnalyser(new QuadGridLayoutAnalyser(m_pModel))
    {
        m_pGridLayout->setContentsMargins(0,0,0,0);
        m_pGridLayout->setSpacing(0);
    }

    ResizeableGridLayoutManager::~ResizeableGridLayoutManager()
    {
        // Buttons are parented to the layout's parent widget, so will be deleted automatically.
    }

    QWidget* ResizeableGridLayoutManager::insertWidget(QWidget *widget, QuadGridLayoutDefs::GridCell cell, Qt::Orientation splitPreference)
    {
        QWidget* previousWidget = m_pModel->widgetAt(cell);

        if ( !m_pModel->canAddWidget(cell) )
        {
            m_pModel->replaceWidget(widget, cell);
        }
        else
        {
            m_pModel->addWidget(widget, cell, splitPreference);
            previousWidget = nullptr;
        }

        updateGridLayout();
        return previousWidget;
    }

    QWidget* ResizeableGridLayoutManager::removeWidget(QuadGridLayoutDefs::GridCell cell, Qt::Orientation mergePreference)
    {
        QWidget* previousWidget = m_pModel->widgetAt(cell);

        if ( m_pModel->canRemoveWidget(cell) )
        {
            m_pModel->removeWidget(cell, mergePreference);
        }

        updateGridLayout();
        return previousWidget;
    }

    void ResizeableGridLayoutManager::equaliseCellSizes()
    {

    }

    void ResizeableGridLayoutManager::resizeButtonDragged(int deltaX, int deltaY)
    {

    }

    void ResizeableGridLayoutManager::updateGridLayout()
    {
        clearGridLayout();

        switch ( m_pModel->widgetCount() )
        {
            case 1:
            {
                setSingleWidgetLayout();
                break;
            }

            case 2:
            case 3:
            case 4:
            {
                genericAddWidgets();
                break;
            }

            default:
            {
                return;
            }
        }

        addResizeButtons();
    }

    void ResizeableGridLayoutManager::addResizeButtons()
    {
        if ( m_pAnalyser->minorSplit() == QuadGridLayoutDefs::MinorNone )
        {
            addMajorSplitOnly();
        }
        else
        {
            addMajorMinorSplit();
        }
    }

    void ResizeableGridLayoutManager::addMajorSplitOnly()
    {
        switch ( m_pAnalyser->majorSplit() )
        {
            case QuadGridLayoutDefs::MajorHorizontal:
            {
                // 0 0 0
                // - - -
                // 0 0 0
                addResizeButton(1, 0, 1, 3);
                break;
            }

            case QuadGridLayoutDefs::MajorVertical:
            {
                // 0 | 0
                // 0 | 0
                // 0 | 0
                addResizeButton(0, 1, 3, 1);
                break;
            }

            case QuadGridLayoutDefs::MajorBoth:
            {
                // 0 | 0
                // - + -
                // 0 | 0
                addResizeButton(0, 1, 1, 1);
                addResizeButton(1, 0, 1, 1);
                addResizeButton(1, 1, 1, 1);
                addResizeButton(1, 2, 1, 1);
                addResizeButton(2, 1, 1, 1);
                break;
            }

            default:
            {
                break;
            }
        }
    }

    void ResizeableGridLayoutManager::addMajorMinorSplit()
    {
        switch ( m_pAnalyser->majorSplit() )
        {
            case QuadGridLayoutDefs::MajorHorizontal:
            {
                // 0 0 0
                // - - -
                // 0 0 0
                addResizeButton(1, 0, 1, 1);
                addResizeButton(1, 1, 1, 1);
                addResizeButton(1, 2, 1, 1);
                break;
            }

            case QuadGridLayoutDefs::MajorVertical:
            {
                // 0 | 0
                // 0 | 0
                // 0 | 0
                addResizeButton(0, 1, 1, 1);
                addResizeButton(1, 1, 1, 1);
                addResizeButton(2, 1, 1, 1);
                break;
            }

            default:
            {
                Q_ASSERT(false);
                break;
            }
        }

        switch ( m_pAnalyser->minorSplit() )
        {
            case QuadGridLayoutDefs::MinorNorth:
            {
                addResizeButton(0, 1, 1, 1);
                break;
            }

            case QuadGridLayoutDefs::MinorEast:
            {
                addResizeButton(1, 2, 1, 1);
                break;
            }

            case QuadGridLayoutDefs::MinorSouth:
            {
                addResizeButton(2, 1, 1, 1);
                break;
            }

            case QuadGridLayoutDefs::MinorWest:
            {
                addResizeButton(1, 0, 1, 1);
                break;
            }

            default:
            {
                Q_ASSERT(false);
                break;
            }
        }
    }

    void ResizeableGridLayoutManager::clearGridLayout()
    {
        QList<QWidget*> widgets = m_pModel->widgets();
        foreach ( QWidget* widget, widgets )
        {
            removeWidgetFromLayout(m_pModel->widgetCells(widget));
        }

        foreach ( ResizeableGridElementButton* button, m_ResizeButtons )
        {
            m_pGridLayout->removeWidget(button);
        }

        qDeleteAll(m_ResizeButtons);
        m_ResizeButtons.clear();
    }

    void ResizeableGridLayoutManager::setSingleWidgetLayout()
    {
        addFullSpanWidgetToLayout(m_pModel->widgetAt(QuadGridLayoutDefs::NorthWest));
    }

    void ResizeableGridLayoutManager::genericAddWidgets()
    {
        QList<QWidget*> widgetList = m_pModel->widgets();
        foreach ( QWidget* widget, widgetList )
        {
            addWidgetToLayout(widget);
        }
        qDebug() << "Done";
    }

    void ResizeableGridLayoutManager::addWidgetToLayout(QWidget *widget)
    {
        QuadGridLayoutModel::GridCellList cellList = m_pModel->widgetCells(widget);
        QuadGridLayoutPoint point(QuadGridLayoutModel::lowestGridCell(cellList));
        QuadGridLayoutDefs::WidgetSpan span = m_pModel->widgetSpan(widget);

        addWidgetToLayout(widget, point, span);
    }

    void ResizeableGridLayoutManager::addFullSpanWidgetToLayout(QWidget *widget)
    {
        addWidgetToLayout(widget, 0, 0, 3, 3);
    }

    void ResizeableGridLayoutManager::addWidgetToLayout(QWidget *widget, const QuadGridLayoutPoint &point, QuadGridLayoutDefs::WidgetSpan span)
    {
        // The grid layout rows/cols are 0 or 2, so we need to adjust this.
        int row = point.y() * 2;
        int col = point.x() * 2;
        int rowSpan = span == QuadGridLayoutDefs::VerticalSpan ? 3 : 1;
        int colSpan = span == QuadGridLayoutDefs::HorizontalSpan ? 3 : 1;

        qDebug() << "Received widget" << widget << "at point" << point << "and with span" << span;
        qDebug() << "Adding widget" << widget << "to layout with row" << row << "column" << col << "row span" << rowSpan << "column span" << colSpan;
        addWidgetToLayout(widget, row, col, rowSpan, colSpan);
    }

    void ResizeableGridLayoutManager::addWidgetToLayout(QWidget *widget, int row, int col, int rowSpan, int colSpan)
    {
        ResizeableGridLayoutContainer* container = new ResizeableGridLayoutContainer();
        container->setItem(widget);
        m_pGridLayout->addWidget(container, row, col, rowSpan, colSpan);
    }

    void ResizeableGridLayoutManager::removeWidgetFromLayout(const QuadGridLayoutModel::GridCellList &list)
    {
        QuadGridLayoutPoint point(QuadGridLayoutModel::lowestGridCell(list));
        QLayoutItem* item = m_pGridLayout->itemAtPosition(point.y(), point.x());
        m_pGridLayout->removeItem(item);
        delete item;
    }

    void ResizeableGridLayoutManager::addResizeButton(int row, int column, int rowSpan, int colSpan)
        {
            ResizeableGridElementButton::ResizeModeFlags flags = ResizeableGridElementButton::NoResizeFlag;

            if ( column == 1 )
            {
                flags |= ResizeableGridElementButton::HorizontalResizeFlag;
            }

            if ( row == 1 )
            {
                flags |= ResizeableGridElementButton::VerticalResizeFlag;
            }

            ResizeableGridElementButton* btn = new ResizeableGridElementButton(flags);
            btn->setRowIndex(row);
            btn->setColumnIndex(column);
            m_pGridLayout->addWidget(btn, row, column, rowSpan, colSpan);
            connect(btn, SIGNAL(mouseMoved(int,int)), this, SLOT(resizeButtonDragged(int,int)));

            m_ResizeButtons.append(btn);
        }
}
