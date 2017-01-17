#include "resizeablegridlayoutmanager.h"
#include <QWidget>
#include <QGridLayout>
#include "resizeablegridelementbutton.h"
#include <QtDebug>
#include "resizeablegridlayoutcontainer.h"

namespace
{
    inline bool isValidIndex(const QPoint& index)
    {
        return index.x() >= 0 && index.y() >= 0;
    }
}

namespace UserInterface
{
    ResizeableGridLayoutManager::ResizeableGridLayoutManager(QGridLayout* gridLayout)
        : m_pGridLayout(gridLayout), m_iLastAddedCell(NoCell)
    {
        initialiseGridLayout();
        resetCellToWidgetMap();
    }

    ResizeableGridLayoutManager::~ResizeableGridLayoutManager()
    {
        // Buttons are parented to the layout's parent widget, so will be deleted automatically.
    }

    void ResizeableGridLayoutManager::resizeButtonDragged(int deltaX, int deltaY)
    {
        qDebug() << "Resize drag:" << deltaX << deltaY;
    }

    QWidget* ResizeableGridLayoutManager::insertWidget(ContentCellFlag cell, QWidget *widget)
    {
        return insertWidget(cell, widget, autoSplitPreference(cell));
    }

    QWidget* ResizeableGridLayoutManager::insertWidget(ContentCellFlag cell, QWidget *widget,
                                                       Qt::Orientation splitPreference)
    {
        if ( !widget || cell == NoCell )
            return nullptr;

        QWidget* existing = getWidget(cell);

        if ( existing )
        {
            ContentCellFlags existingFlags = getFlags(existing);
            if ( isSingleFlag(existingFlags) )
            {
                ContentCellFlag flag = static_cast<ContentCellFlag>(static_cast<int>(existingFlags));
                QPoint index = flagToIndex(flag);
                replaceWidgetInLayout(widget, index.y(), index.x());
                swapWidgetFlags(existing, widget);
            }
            else
            {
                splitWidget(existing, widget, cell, splitPreference);
                existing = nullptr;
            }
        }
        else
        {
            setSingleWidget(widget);
        }

        m_iLastAddedCell = cell;
        rebuildResizeButtons();
        return existing;
    }

    void ResizeableGridLayoutManager::setSingleWidget(QWidget *widget)
    {
        //m_pGridLayout->addWidget(widget, 0, 0, 3, 3); TODO: Remove
        insertWidgetIntoLayout(widget, 0, 0, 3, 3);

        m_CellToWidget[UpperLeft] = widget;
        m_CellToWidget[UpperRight] = widget;
        m_CellToWidget[LowerLeft] = widget;
        m_CellToWidget[LowerRight] = widget;

        m_WidgetToCell.clear();
        m_WidgetToCell.insert(widget, ContentCellFlags() | UpperLeft | UpperRight | LowerLeft | LowerRight);

        setHandleLayout(ResizeHandleLayout::None);
    }

    void ResizeableGridLayoutManager::splitWidget(QWidget *existing, QWidget *widget, ContentCellFlag cell,
                                                  Qt::Orientation splitPreference)
    {
        Q_ASSERT(existing);
        Q_ASSERT(widget);
        Q_ASSERT(isSingleFlag(cell));

        switch ( cellCount(getFlags(existing)) )
        {
            case 2:
            {
                transitionSplitRectWidget(existing, widget, cell);
                break;
            }

            case 4:
            {
                SplitIndexChoice choice = indexChoice(splitPreference, cell);
                transition1to2widgets(existing, widget, splitPreference, choice);
                break;
            }

            default:
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Unexpected number of cells occupied by widget to split!");
                break;
            }
        }
    }

    void ResizeableGridLayoutManager::transition1to2widgets(QWidget* existing, QWidget *widget,
                                                            Qt::Orientation split,
                                                            SplitIndexChoice newWidgetIndex)
    {
        //m_pGridLayout->removeWidget(existing); // TODO: Remove
        removeWidgetFromLayout(existing);

        setHandleLayout(ResizeHandleLayout::Bar);
        m_HandleLayout.orientation = split;

        QWidget* lower = newWidgetIndex == LowerIndex
                ? widget
                : existing;

        QWidget* upper = newWidgetIndex == LowerIndex
                ? existing
                : widget;

        if ( split == Qt::Horizontal )
        {
            // TODO: Refactor into a function
            //m_pGridLayout->addWidget(lower, 0, 0, 1, 3); TODO: Remove
            //m_pGridLayout->addWidget(upper, 2, 0, 1, 3); TODO: Remove
            insertWidgetIntoLayout(lower, 0, 0, 1, 3);
            insertWidgetIntoLayout(upper, 2, 0, 1, 3);

            m_CellToWidget[UpperLeft] = lower;
            m_CellToWidget[UpperRight] = lower;
            m_CellToWidget[LowerLeft] = upper;
            m_CellToWidget[LowerRight] = upper;

            m_WidgetToCell.insert(lower, ContentCellFlags() | UpperLeft | UpperRight);
            m_WidgetToCell.insert(upper, ContentCellFlags() | LowerLeft | LowerRight);

            m_pGridLayout->setRowStretch(0, 1);
            m_pGridLayout->setRowStretch(2, 1);

            m_pGridLayout->setColumnStretch(0, 1);
            m_pGridLayout->setColumnStretch(2, 0);
        }
        else
        {
            // TODO: Refactor into a function
            //m_pGridLayout->addWidget(lower, 0, 0, 3, 1); TODO: Remove
            //m_pGridLayout->addWidget(upper, 0, 2, 3, 1); TODO: Remove
            insertWidgetIntoLayout(lower, 0, 0, 3, 1);
            insertWidgetIntoLayout(upper, 0, 2, 3, 1);

            m_CellToWidget[UpperLeft] = lower;
            m_CellToWidget[LowerLeft] = lower;
            m_CellToWidget[UpperRight] = upper;
            m_CellToWidget[LowerRight] = upper;

            m_WidgetToCell.insert(lower, ContentCellFlags() | UpperLeft | LowerLeft);
            m_WidgetToCell.insert(upper, ContentCellFlags() | UpperRight | LowerRight);

            m_pGridLayout->setRowStretch(0, 1);
            m_pGridLayout->setRowStretch(2, 0);

            m_pGridLayout->setColumnStretch(0, 1);
            m_pGridLayout->setColumnStretch(2, 1);
        }
    }

    void ResizeableGridLayoutManager::transitionSplitRectWidget(QWidget *existing, QWidget *widget, ContentCellFlag cell)
    {

        ContentCellFlags existingFlags = getFlags(existing);

        Q_ASSERT_X(existingFlags.testFlag(cell),
                   Q_FUNC_INFO,
                   "Expected target cell to be occupied by existing cell!");

        existingFlags &= ~cell;

        Q_ASSERT_X(isSingleFlag(existingFlags),
                   Q_FUNC_INFO,
                   "Expected existing widget to occupy two cells!");

        //m_pGridLayout->removeWidget(existing); TODO: Remove
        removeWidgetFromLayout(existing);

        QPoint existingIndex = flagToIndex(existingFlags);
        //m_pGridLayout->addWidget(existing, existingIndex.y(), existingIndex.x()); TODO: Remove
        insertWidgetIntoLayout(existing, existingIndex.y(), existingIndex.x());

        ContentCellFlag existingFlag = singleFlag(existingFlags);
        m_CellToWidget[existingFlag] = existing;
        m_WidgetToCell[existing] = existingFlag;

        QPoint newIndex = cellToIndex(cell);
        //m_pGridLayout->addWidget(widget, newIndex.y(), newIndex.x()); TODO: Remove
        insertWidgetIntoLayout(widget, newIndex.y(), newIndex.x());

        m_CellToWidget[cell] = widget;
        m_WidgetToCell[widget] = cell;

        Qt::Orientation majorSplit = newIndex.y() == existingIndex.y() ? Qt::Horizontal : Qt::Vertical;
        setHalfSplitStretch(majorSplit);

        setHandleLayout(m_WidgetToCell.count() == 4 ? ResizeHandleLayout::Quad : ResizeHandleLayout::Tri);
        m_HandleLayout.orientation = majorSplit;
        m_HandleLayout.tJunction = tJunctionIndex(existingFlag, cell);
    }

    bool ResizeableGridLayoutManager::isSingleCellWidget(QWidget *widget) const
    {
        if ( !widget )
            return false;

        return isSingleFlag(m_WidgetToCell.value(widget, NoCell));
    }

    void ResizeableGridLayoutManager::resetCellToWidgetMap()
    {
        m_CellToWidget.clear();
        m_CellToWidget.insert(UpperLeft, nullptr);
        m_CellToWidget.insert(LowerLeft, nullptr);
        m_CellToWidget.insert(UpperRight, nullptr);
        m_CellToWidget.insert(LowerLeft, nullptr);
    }

    void ResizeableGridLayoutManager::initialiseGridLayout()
    {
        m_pGridLayout->setColumnStretch(1, 0);
        m_pGridLayout->setRowStretch(1, 0);
        m_pGridLayout->setContentsMargins(0,0,0,0);
        m_pGridLayout->setSpacing(0);
    }

    QWidget* ResizeableGridLayoutManager::getWidget(ContentCellFlag flag) const
    {
        return m_CellToWidget.value(flag, nullptr);
    }

    ResizeableGridLayoutManager::ContentCellFlags ResizeableGridLayoutManager::getFlags(QWidget *widget) const
    {
        return m_WidgetToCell.value(widget, NoCell);
    }

    bool ResizeableGridLayoutManager::isSingleFlag(int cell)
    {
        switch (cell)
        {
            case UpperLeft:
            case UpperRight:
            case LowerLeft:
            case LowerRight:
                return true;

            default:
                return false;
        }
    }

    QPoint ResizeableGridLayoutManager::flagToIndex(const ContentCellFlags &flags)
    {
        if ( !isSingleFlag(flags) )
            return QPoint(-1,-1);

        return cellToIndex(static_cast<ContentCellFlag>(static_cast<int>(flags)));
    }

    QPoint ResizeableGridLayoutManager::cellToIndex(ContentCellFlag cell)
    {
        switch (cell)
        {
            case UpperLeft:
                return QPoint(0,0);

            case UpperRight:
                return QPoint(2,0);

            case LowerLeft:
                return QPoint(0,2);

            case LowerRight:
                return QPoint(2,2);

            default:
                return QPoint(-1,-1);
        }
    }

    int ResizeableGridLayoutManager::cellCount(const ContentCellFlags &flags)
    {
        int count = 0;

        if ( flags.testFlag(UpperLeft) )
            ++count;

        if ( flags.testFlag(LowerLeft) )
            ++count;

        if ( flags.testFlag(UpperRight) )
            ++count;

        if ( flags.testFlag(LowerRight) )
            ++count;

        return count;
    }

    ResizeableGridLayoutManager::SplitIndexChoice ResizeableGridLayoutManager::indexChoice(
            Qt::Orientation splitPreference, ContentCellFlag cell)
    {
        if ( splitPreference == Qt::Horizontal )
        {
            // Upper cells have a lower row index.
            return (cell == UpperLeft || cell == UpperRight)
                    ? LowerIndex
                    : UpperIndex;

        }
        else
        {
            // Left cells have a lower column index.
            return (cell == UpperLeft || cell == LowerLeft)
                    ? LowerIndex
                    : UpperIndex;
        }
    }

    void ResizeableGridLayoutManager::addVerticalResizeButton()
    {
        addResizeButton(0, 1, 3, 1);
    }

    void ResizeableGridLayoutManager::addHorizontalResizeButton()
    {
        addResizeButton(1, 0, 1, 3);
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

    void ResizeableGridLayoutManager::rebuildResizeButtons()
    {
        removeResizeButtons();

        switch ( m_HandleLayout.type )
        {
            case ResizeHandleLayout::Bar:
            {
                rebuildHalfResizeButtons();
                break;
            }

            case ResizeHandleLayout::Tri:
            {
                rebuildTResizeButtons();
                break;
            }

            case ResizeHandleLayout::Quad:
            {
                rebuildQuadResizeButtons();
                break;
            }

            default:
            {
                break;
            }
        }
    }

    void ResizeableGridLayoutManager::rebuildHalfResizeButtons()
    {
        if ( m_HandleLayout.orientation == Qt::Horizontal )
            addHorizontalResizeButton();
        else
            addVerticalResizeButton();
    }

    void ResizeableGridLayoutManager::rebuildTResizeButtons()
    {
        rebuildHalfResizeButtons();
        qDebug() << "Adding T-junction at row" << m_HandleLayout.tJunction.y()
                 << "col" << m_HandleLayout.tJunction.x();
        addResizeButton(m_HandleLayout.tJunction.y(), m_HandleLayout.tJunction.x(), 1, 1);
    }

    void ResizeableGridLayoutManager::rebuildQuadResizeButtons()
    {
        addResizeButton(0, 1, 1, 1);
        addResizeButton(1, 0, 1, 1);
        addResizeButton(1, 1, 1, 1);
        addResizeButton(1, 2, 1, 1);
        addResizeButton(2, 1, 1, 1);
    }

    void ResizeableGridLayoutManager::removeResizeButtons()
    {
        foreach ( ResizeableGridElementButton* button, m_ResizeButtons )
        {
            m_pGridLayout->removeWidget(button);
            delete button;
        }

        m_ResizeButtons.clear();
    }

    QList<ResizeableGridLayoutManager::ContentCellFlag>
        ResizeableGridLayoutManager::flagList(const ContentCellFlags &flags)
    {
        QList<ContentCellFlag> list;

        if ( flags.testFlag(UpperLeft) )
            list.append(UpperLeft);

        if ( flags.testFlag(UpperRight) )
            list.append(UpperRight);

        if ( flags.testFlag(LowerLeft) )
            list.append(LowerLeft);

        if ( flags.testFlag(LowerRight) )
            list.append(LowerRight);

        return list;
    }

    ResizeableGridLayoutManager::ContentCellFlag ResizeableGridLayoutManager::singleFlag(const ContentCellFlags &flags)
    {
        if ( !isSingleFlag(flags) )
            return NoCell;

        QList<ContentCellFlag> list = flagList(flags);
        if ( list.isEmpty() )
            return NoCell;

        return list.at(0);
    }

    void ResizeableGridLayoutManager::swapWidgetFlags(QWidget *existing, QWidget *widget)
    {
        ContentCellFlags flags = getFlags(existing);
        QList<ContentCellFlag> list = flagList(flags);

        foreach ( ContentCellFlag f, list )
        {
            m_CellToWidget[f] = widget;
        }

        m_WidgetToCell.remove(existing);
        m_WidgetToCell.insert(widget, flags);
    }

    void ResizeableGridLayoutManager::setHalfSplitStretch(Qt::Orientation splitDir)
    {
        if ( splitDir == Qt::Horizontal )
        {
            m_pGridLayout->setRowStretch(0, 1);
            m_pGridLayout->setRowStretch(2, 1);
            m_pGridLayout->setColumnStretch(0, 1);
            m_pGridLayout->setColumnStretch(2, 0);
        }
        else
        {
            m_pGridLayout->setColumnStretch(0, 1);
            m_pGridLayout->setColumnStretch(2, 1);
            m_pGridLayout->setRowStretch(0, 1);
            m_pGridLayout->setRowStretch(2, 1);
        }
    }

    void ResizeableGridLayoutManager::removeWidget(QWidget *widget)
    {
        if ( !widget )
            return;

        QList<ContentCellFlag> flags = flagList(getFlags(widget));
        if ( flags.isEmpty() )
            return;

        QPoint index = flagToIndex(flags.at(0));
        if ( isValidIndex(index) )
            return;

        m_pGridLayout->removeItem(m_pGridLayout->itemAtPosition(index.y(), index.x()));
    }

    Qt::Orientation ResizeableGridLayoutManager::autoSplitPreference(ContentCellFlag newCell)
    {
        QPoint i = flagToIndex(m_iLastAddedCell);
        QPoint j = flagToIndex(newCell);

        if ( !isValidIndex(i) || !isValidIndex(j) )
            return Qt::Horizontal;

        if ( i.y() == j.y() )
            return Qt::Vertical;

        return Qt::Horizontal;
    }

    void ResizeableGridLayoutManager::setHandleLayout(ResizeHandleLayout::LayoutType type)
    {
        m_HandleLayout = ResizeHandleLayout();
        m_HandleLayout.type = type;
    }

    QPoint ResizeableGridLayoutManager::tJunctionIndex(ContentCellFlag singleCell1, ContentCellFlag singleCell2)
    {
        const ContentCellFlag& lower = singleCell1 < singleCell2 ? singleCell1 : singleCell2;
        const ContentCellFlag& upper = singleCell1 < singleCell2 ? singleCell2 : singleCell1;

        if ( lower == UpperLeft && upper == UpperRight )
        {
            return QPoint(1, 0);
        }
        else if ( lower == UpperRight && upper == LowerRight )
        {
            return QPoint(2, 1);
        }
        else if ( lower == LowerLeft && upper == LowerRight )
        {
            return QPoint(1, 2);
        }
        else
        {
            return QPoint(0, 1);
        }
    }

    void ResizeableGridLayoutManager::insertWidgetIntoLayout(QWidget *widget, int row, int col, int rowSpan, int colSpan)
    {
        ResizeableGridLayoutContainer* container = new ResizeableGridLayoutContainer();
        container->setItem(widget);
        m_pGridLayout->addWidget(container, row, col, rowSpan, colSpan);
    }

    void ResizeableGridLayoutManager::removeWidgetFromLayout(QWidget *widget)
    {
        if ( !widget )
            return;

        for ( int row = 0; row <= 2; row += 2 )
        {
            for ( int col = 0; col <= 2; col += 2 )
            {
                QLayoutItem* item = m_pGridLayout->itemAtPosition(row, col);
                if ( !item )
                    continue;

                QWidget* containerWidget = item->widget();
                if ( !containerWidget )
                    continue;

                ResizeableGridLayoutContainer* container = qobject_cast<ResizeableGridLayoutContainer*>(containerWidget);
                if ( !container )
                    continue;

                if ( container->item() == widget )
                {
                    m_pGridLayout->removeWidget(container);
                    container->replaceItem(nullptr);
                    widget->setParent(nullptr);
                    delete container;
                    return;
                }
            }
        }
    }

    QWidget* ResizeableGridLayoutManager::replaceWidgetInLayout(QWidget* newWidget, int row, int col)
    {
        if ( !newWidget )
            return nullptr;

        QLayoutItem* item = m_pGridLayout->itemAtPosition(row, col);
        if ( !item )
            return nullptr;

        QWidget* containerWidget = item->widget();
        if ( !containerWidget )
            return nullptr;

        ResizeableGridLayoutContainer* container = qobject_cast<ResizeableGridLayoutContainer*>(containerWidget);
        if ( !container )
            return nullptr;

        return container->replaceItem(newWidget);
    }
}
