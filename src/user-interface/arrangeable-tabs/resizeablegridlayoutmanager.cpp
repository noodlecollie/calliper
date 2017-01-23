#include "resizeablegridlayoutmanager.h"
#include <QGridLayout>
#include <QWidget>
#include "resizeablegridlayoutcontainer.h"
#include "resizeablegridelementbutton.h"

namespace
{
    inline int clampAbove(int in, int threshold)
    {
        return in < threshold ? threshold : in;
    }
}

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

        m_pGridLayout->setRowStretch(1, 0);
        m_pGridLayout->setColumnStretch(1, 0);

        m_pGridLayout->setRowMinimumHeight(0, 1);
        m_pGridLayout->setRowMinimumHeight(2, 1);

        m_pGridLayout->setColumnMinimumWidth(0, 1);
        m_pGridLayout->setColumnMinimumWidth(2, 1);
    }

    ResizeableGridLayoutManager::~ResizeableGridLayoutManager()
    {
        // Buttons are parented to the layout's parent widget, so will be deleted automatically.
    }

    QWidget* ResizeableGridLayoutManager::insertWidget(QWidget *widget, QuadGridLayoutDefs::GridCell cell, Qt::Orientation splitPreference)
    {
        if ( !widget )
            return nullptr;

        clearGridLayout();

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
        updateStretchFactors();
        return previousWidget;
    }

    QWidget* ResizeableGridLayoutManager::removeWidget(QuadGridLayoutDefs::GridCell cell, Qt::Orientation mergePreference)
    {
        if ( !m_pModel->canRemoveWidget(cell) )
        {
            return nullptr;
        }

        clearGridLayout();

        QWidget* previousWidget = m_pModel->widgetAt(cell);
        m_pModel->removeWidget(cell, mergePreference);

        updateGridLayout();
        return previousWidget;
    }

    bool ResizeableGridLayoutManager::removeWidget(QWidget *widget, Qt::Orientation mergePreference)
    {
        if ( !widget )
            return false;

        QuadGridLayoutModel::GridCellList list = m_pModel->widgetCells(widget);
        if ( list.isEmpty() )
            return false;

        QWidget* removed = removeWidget(QuadGridLayoutModel::lowestGridCell(list), mergePreference);
        return removed == widget;
    }

    void ResizeableGridLayoutManager::equaliseCellSizes()
    {
        switch ( m_pModel->widgetCount() )
        {
            case 2:
            {
                if ( m_pAnalyser->majorSplit() == QuadGridLayoutDefs::MajorHorizontal )
                {
                    setStretchFactors(1, 1, 0, 0);
                }
                else
                {
                    setStretchFactors(0, 0, 1, 1);
                }

                break;
            }

            case 3:
            case 4:
            {
                setStretchFactors(1, 1, 1, 1);
                break;
            }

            default:
            {
                break;
            }
        }
    }

    void ResizeableGridLayoutManager::maximizeWidget(QWidget *widget)
    {
        if ( !widget )
            return;

        QuadGridLayoutModel::GridCellList list = m_pModel->widgetCells(widget);
        if ( list.isEmpty() )
            return;

        maximizeWidget(QuadGridLayoutModel::lowestGridCell(list));
    }

    void ResizeableGridLayoutManager::maximizeWidget(QuadGridLayoutDefs::GridCell cell)
    {
        QuadGridLayoutPoint point(cell);
        QuadGridLayoutPoint opposite = point.diagonalNeighbour();
        m_pGridLayout->setRowStretch(point.y() * 2, 1);
        m_pGridLayout->setRowStretch(opposite.y() * 2, 0);
        m_pGridLayout->setColumnStretch(point.x() * 2, 1);
        m_pGridLayout->setColumnStretch(opposite.x() * 2, 0);
    }

    void ResizeableGridLayoutManager::resizeButtonDragged(int deltaX, int deltaY)
    {
        ResizeableGridElementButton* button = qobject_cast<ResizeableGridElementButton*>(sender());
        if ( !button )
            return;

        if ( button->resizeFlags().testFlag(ResizeableGridElementButton::HorizontalResizeFlag) )
        {
            resizeHorizontal(deltaX);
        }

        if ( button->resizeFlags().testFlag(ResizeableGridElementButton::VerticalResizeFlag) )
        {
            resizeVertical(deltaY);
        }
    }

    void ResizeableGridLayoutManager::updateGridLayout()
    {
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
        QLayoutItem *child;
        while ( (child = m_pGridLayout->takeAt(0)) != nullptr )
        {
            if ( cleanUpContainer(qobject_cast<ResizeableGridLayoutContainer*>(child->widget())) )
            {
                delete child;
                continue;
            }

            if ( cleanUpButton(qobject_cast<ResizeableGridElementButton*>(child->widget())) )
            {
                delete child;
                continue;
            }

            delete child;
        }

        m_ResizeButtons.clear();
    }

    bool ResizeableGridLayoutManager::cleanUpContainer(ResizeableGridLayoutContainer *container)
    {
        if ( !container )
            return false;

        container->replaceItem(nullptr);
        delete container;
        return true;
    }

    bool ResizeableGridLayoutManager::cleanUpButton(ResizeableGridElementButton *button)
    {
        if ( !button )
            return false;

        delete button;
        return true;
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

        addWidgetToLayout(widget, row, col, rowSpan, colSpan);
    }

    void ResizeableGridLayoutManager::addWidgetToLayout(QWidget *widget, int row, int col, int rowSpan, int colSpan)
    {
        ResizeableGridLayoutContainer* container = new ResizeableGridLayoutContainer();
        container->setItem(widget);

        connect(container, SIGNAL(handleDoubleClicked()), this, SLOT(containerFloatClicked()));
        connect(container, SIGNAL(closeClicked()), this, SLOT(containerCloseClicked()));
        connect(container, SIGNAL(maximizeClicked()), this, SLOT(containerMaximizeClicked()));
        connect(container, SIGNAL(floatClicked(QWidget*)), this, SIGNAL(widgetFloatClicked(QWidget*)));

        m_pGridLayout->addWidget(container, row, col, rowSpan, colSpan);
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

    void ResizeableGridLayoutManager::updateStretchFactors()
    {
        switch ( m_pModel->widgetCount() )
        {
            case 1:
            {
                setSingleWidgetStretchFactors();
                break;
            }

            case 2:
            {
                setDualWidgetStretchFactors();
                break;
            }

            case 3:
            {
                setTripleWidgetStretchFactors();
                break;
            }

            default:
            {
                break;
            }
        }
    }

    void ResizeableGridLayoutManager::setSingleWidgetStretchFactors()
    {
        setStretchFactors(1, 0, 1, 0);
    }

    void ResizeableGridLayoutManager::setDualWidgetStretchFactors()
    {
        if ( m_pAnalyser->majorSplit() == QuadGridLayoutDefs::MajorVertical )
        {
            setStretchFactors(1, 0, 1, 1);
        }
        else
        {
            setStretchFactors(1, 1, 1, 0);
        }
    }

    void ResizeableGridLayoutManager::setTripleWidgetStretchFactors()
    {
        switch ( m_pAnalyser->minorSplit() )
        {
            case QuadGridLayoutDefs::MinorNorth:
            case QuadGridLayoutDefs::MinorSouth:
            {
                m_pGridLayout->setColumnStretch(0, 1);
                m_pGridLayout->setColumnStretch(2, 1);
                break;
            }

            case QuadGridLayoutDefs::MinorWest:
            case QuadGridLayoutDefs::MinorEast:
            {
                m_pGridLayout->setRowStretch(0, 1);
                m_pGridLayout->setRowStretch(2, 1);
                break;
            }

            default:
            {
                break;
            }
        }
    }

    void ResizeableGridLayoutManager::setStretchFactors(int row0, int row2, int col0, int col2)
    {
        m_pGridLayout->setRowStretch(0, row0);
        m_pGridLayout->setRowStretch(2, row2);
        m_pGridLayout->setColumnStretch(0, col0);
        m_pGridLayout->setColumnStretch(2, col2);
    }

    void ResizeableGridLayoutManager::resizeHorizontal(int delta)
    {
        QRect leftRect = m_pGridLayout->cellRect(0,0);
        QRect rightRect = m_pGridLayout->cellRect(0,2);

        if ( !leftRect.isValid() || !rightRect.isValid() )
            return;

        int left = clampAbove(leftRect.width() + delta, 1);
        int right = clampAbove(rightRect.width() - delta, 1);

        m_pGridLayout->setColumnStretch(0, left);
        m_pGridLayout->setColumnStretch(2, right);
    }

    void ResizeableGridLayoutManager::resizeVertical(int delta)
    {
        QRect topRect = m_pGridLayout->cellRect(0,0);
        QRect bottomRect = m_pGridLayout->cellRect(2,0);

        if ( !topRect.isValid() || !bottomRect.isValid() )
            return;

        int top = clampAbove(topRect.height() + delta, 1);
        int bottom = clampAbove(bottomRect.height() - delta, 1);

        m_pGridLayout->setRowStretch(0, top);
        m_pGridLayout->setRowStretch(2, bottom);
    }

    void ResizeableGridLayoutManager::containerCloseClicked()
    {
        ResizeableGridLayoutContainer* container = qobject_cast<ResizeableGridLayoutContainer*>(sender());
        Q_ASSERT(container);

        QWidget* widget = container->item();
        if ( !widget )
            return;

        removeWidget(widget, Qt::Horizontal);
        delete widget;
    }

    void ResizeableGridLayoutManager::containerMaximizeClicked()
    {
        ResizeableGridLayoutContainer* container = qobject_cast<ResizeableGridLayoutContainer*>(sender());
        Q_ASSERT(container);

        QWidget* widget = container->item();
        if ( !widget )
            return;

        maximizeWidget(widget);
    }
}
