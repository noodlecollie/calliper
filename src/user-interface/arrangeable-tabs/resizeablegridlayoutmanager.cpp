#include "resizeablegridlayoutmanager.h"
#include <QWidget>
#include <QGridLayout>
#include "resizeablegridelementbutton.h"
#include <QtDebug>

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
        : m_pGridLayout(gridLayout)
    {
        initialiseGridLayout();
        resetCellToWidgetMap();
    }

    ResizeableGridLayoutManager::~ResizeableGridLayoutManager()
    {
        // Buttons are parented to the layout's parent widget, so will be deleted automatically.
    }

    QWidget* ResizeableGridLayoutManager::insertWidget(ContentCellFlag cell, QWidget *widget,
                                                       Qt::Orientation splitPreference)
    {
        if ( !widget )
            return nullptr;

        QWidget* existing = getWidget(cell);

        if ( existing )
        {
            if ( isSingleFlag(getFlags(existing)) )
            {
                m_pGridLayout->replaceWidget(existing, widget, Qt::FindDirectChildrenOnly);
                swapWidgetFlags(existing, widget);
            }
            else
            {
                removeResizeButtons();
                splitWidget(existing, widget, cell, splitPreference);
            }

            return existing;
        }

        removeResizeButtons();

        // TODO - complete
        Q_ASSERT(false);
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
        m_pGridLayout->removeWidget(existing);

        QWidget* lower = newWidgetIndex == LowerIndex
                ? widget
                : existing;

        QWidget* upper = newWidgetIndex == LowerIndex
                ? existing
                : widget;

        if ( split == Qt::Horizontal )
        {
            // TODO: Refactor into a function
            m_pGridLayout->addWidget(lower, 0, 0, 1, 3);
            m_pGridLayout->addWidget(upper, 2, 0, 1, 3);

            m_CellToWidget[UpperLeft] = lower;
            m_CellToWidget[UpperRight] = lower;
            m_CellToWidget[LowerLeft] = upper;
            m_CellToWidget[LowerRight] = upper;

            setFlags(lower, UpperLeft | UpperRight);
            setFlags(upper, LowerLeft | LowerRight);

            m_pGridLayout->setRowStretch(0, 1);
            m_pGridLayout->setRowStretch(2, 1);

            m_pGridLayout->setColumnStretch(0, 1);
            m_pGridLayout->setColumnStretch(2, 0);

            addHorizontalResizeButton();
        }
        else
        {
            // TODO: Refactor into a function
            m_pGridLayout->addWidget(lower, 0, 0, 3, 1);
            m_pGridLayout->addWidget(upper, 0, 2, 3, 1);

            m_CellToWidget[UpperLeft] = lower;
            m_CellToWidget[LowerLeft] = lower;
            m_CellToWidget[UpperRight] = upper;
            m_CellToWidget[LowerRight] = upper;

            setFlags(lower, UpperLeft | LowerLeft);
            setFlags(upper, UpperRight | LowerRight);

            m_pGridLayout->setRowStretch(0, 1);
            m_pGridLayout->setRowStretch(2, 0);

            m_pGridLayout->setColumnStretch(0, 1);
            m_pGridLayout->setColumnStretch(2, 1);

            addVerticalResizeButton();
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

        m_pGridLayout->removeWidget(existing);

        QPoint existingIndex = flagToIndex(existingFlags);
        m_pGridLayout->addWidget(existing, existingIndex.y(), existingIndex.x());
        m_CellToWidget[singleFlag(existingFlags)] = existing;

        QPoint newIndex = cellToIndex(cell);
        m_pGridLayout->addWidget(widget, newIndex.y(), newIndex.x());
        m_CellToWidget[cell] = widget;
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

    void ResizeableGridLayoutManager::removeResizeButtons()
    {
        removeResizeButton(0, 1);
        removeResizeButton(1, 0);
        removeResizeButton(1, 1);
        removeResizeButton(1, 2);
        removeResizeButton(2, 1);
    }

    void ResizeableGridLayoutManager::removeResizeButton(int row, int col)
    {
        QLayoutItem* item = m_pGridLayout->itemAtPosition(row, col);
        if ( !item )
            return;

        QWidget* widget = item->widget();
        if ( !widget )
            return;

        m_pGridLayout->removeWidget(widget);
        delete widget;
    }

    QList<ResizeableGridLayoutManager::ContentCellFlag>
        ResizeableGridLayoutManager::flagList(const ContentCellFlags &flags)
    {
        QList list;

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

        QList list = flagList(flags);
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

    void ResizeableGridLayoutManager::setFlags(QWidget *widget, int flags)
    {
        m_WidgetToCell.insert(widget, ContentCellFlags() & flags);
    }
}
