#include "resizeablegridlayoutmanager.h"
#include <QWidget>
#include <QGridLayout>
#include "resizeablegridelementbutton.h"
#include <QtDebug>

namespace
{
    inline QPoint indexForCell(UserInterface::ResizeableGridLayoutManager::ContentCell cell)
    {
        return QPoint(static_cast<int>((cell & 0xffff0000) >> 16),
                      static_cast<int>(cell & 0x0000ffff));
    }

    inline int clampAbove(int in, int threshold)
    {
        return in < threshold ? threshold : in;
    }
}

namespace UserInterface
{
    ResizeableGridLayoutManager::ResizeableGridLayoutManager(QGridLayout* gridLayout)
        : m_pGridLayout(gridLayout),
          m_OccupiedCells(),
          m_iLastCell(NoCell),
          m_ResizeButtons()
    {
        m_pGridLayout->setColumnStretch(1, 0);
        m_pGridLayout->setRowStretch(1, 0);
        m_pGridLayout->setContentsMargins(0,0,0,0);
        m_pGridLayout->setSpacing(0);
    }

    ResizeableGridLayoutManager::~ResizeableGridLayoutManager()
    {
        // Buttons are parented to the layout's parent widget, so will be deleted automatically.
    }

    QWidget* ResizeableGridLayoutManager::widgetAt(ContentCell cell) const
    {
        return m_OccupiedCells.value(cell, nullptr);
    }

    void ResizeableGridLayoutManager::addWidget(QWidget *widget, ContentCell cell)
    {
        if ( cell == NoCell || m_OccupiedCells.contains(cell) )
            return;

        m_OccupiedCells.insert(cell, widget);
        m_iLastCell = cell;
        updateLayout();
    }

    QWidget* ResizeableGridLayoutManager::takeWidget(ContentCell cell)
    {
        if ( !m_OccupiedCells.contains(cell) )
            return nullptr;

        QWidget* w = m_OccupiedCells.value(cell);
        m_OccupiedCells.remove(cell);
        QPoint index = indexForCell(cell);
        m_pGridLayout->removeItem(m_pGridLayout->itemAtPosition(index.y(), index.x()));
        return w;
    }

    void ResizeableGridLayoutManager::updateLayout()
    {
        // For now, completely rebuild the layout.
        // This may need to be optimised later.
        clearLayout();

        switch ( m_OccupiedCells.count() )
        {
            case 0:
                // Nothing else to do.
                break;

            case 1:
                setSingleItemLayout();
                break;

            case 2:
                setDualItemLayout();
                break;

            case 3:
                setTripleItemLayout();
                break;

            case 4:
                setQuadItemLayout();
                break;

            default:
                Q_ASSERT_X(false, Q_FUNC_INFO, "Cell hash contains more than 4 items!");
                break;
        }
    }

    void ResizeableGridLayoutManager::clearLayout()
    {
        for ( int row = 0; row < m_pGridLayout->rowCount(); row++ )
        {
            for ( int column = 0; column < m_pGridLayout->columnCount(); column++ )
            {
                m_pGridLayout->removeItem(m_pGridLayout->itemAtPosition(row, column));
            }
        }

        qDeleteAll(m_ResizeButtons);
        m_ResizeButtons.clear();
        setRowColMinSize();
    }

    void ResizeableGridLayoutManager::setSingleItemLayout()
    {
        Q_ASSERT_X(m_OccupiedCells.count() == 1, Q_FUNC_INFO, "Expected a single widget!");

        QWidget* widget = m_OccupiedCells.begin().value();
        m_pGridLayout->addWidget(widget, 0, 0);
        setStretchFactors(1,0,1,0);
    }

    void ResizeableGridLayoutManager::setDualItemLayout()
    {
        Q_ASSERT_X(m_OccupiedCells.count() == 2, Q_FUNC_INFO, "Expected two widgets!");

        ContentCell lowerCell, upperCell;
        bool splitIsVertical = calculateDualItemSplit(m_OccupiedCells.constBegin().key(),
                                                      (++m_OccupiedCells.constBegin()).key(),
                                                      lowerCell,
                                                      upperCell);

        if ( splitIsVertical )
        {
            m_pGridLayout->addWidget(m_OccupiedCells.value(lowerCell), 0, 0, 3, 1);
            m_pGridLayout->addWidget(m_OccupiedCells.value(upperCell), 0, 2, 3, 1);
            addResizeButton(0,1,3,1);
            setStretchFactors(1,0,1,1);
        }
        else
        {
            m_pGridLayout->addWidget(m_OccupiedCells.value(lowerCell), 0, 0, 1, 3);
            m_pGridLayout->addWidget(m_OccupiedCells.value(upperCell), 2, 0, 1, 3);
            addResizeButton(1,0,1,3);
            setStretchFactors(1,1,1,0);
        }
    }

    void ResizeableGridLayoutManager::setTripleItemLayout()
    {
        Q_ASSERT_X(m_OccupiedCells.count() == 3, Q_FUNC_INFO, "Expected three widgets!");

        // The layout of the three cells depends on where the new one is added.
        // Work out the layout of the previous two first.

        QList<ContentCell> cellList;
        for( QHash<ContentCell, QWidget*>::const_iterator it = m_OccupiedCells.constBegin();
             it != m_OccupiedCells.constEnd();
             ++it)
        {
            if ( it.key() != m_iLastCell )
                cellList.append(it.key());
        }

        Q_ASSERT(cellList.count() == 2);

        ContentCell lowerCell, upperCell;
        bool splitIsVertical = calculateDualItemSplit(cellList.at(0),
                                                      cellList.at(1),
                                                      lowerCell,
                                                      upperCell);

        QPoint latestIndex = indexForCell(m_iLastCell);
        ContentCell extendedCell, shortenedCell;
        if ( splitIsVertical )
        {
            if ( latestIndex.x() == 0 )
            {
                extendedCell = upperCell;
                shortenedCell = lowerCell;

                m_pGridLayout->addWidget(m_OccupiedCells.value(extendedCell), 0, 2, 3, 1);
                addResizeButton(1, 0);
            }
            else
            {
                extendedCell = lowerCell;
                shortenedCell = upperCell;

                m_pGridLayout->addWidget(m_OccupiedCells.value(extendedCell), 0, 0, 3, 1);
                addResizeButton(1, 2);
            }

            addResizeButton(0, 1);
            addResizeButton(2, 1);

            m_pGridLayout->setRowStretch(0,1);
            m_pGridLayout->setRowStretch(2,1);
        }
        else
        {
            if ( latestIndex.y() == 0 )
            {
                extendedCell = upperCell;
                shortenedCell = lowerCell;

                m_pGridLayout->addWidget(m_OccupiedCells.value(extendedCell), 2, 0, 1, 3);
                addResizeButton(0, 1);
            }
            else
            {
                extendedCell = lowerCell;
                shortenedCell = upperCell;

                m_pGridLayout->addWidget(m_OccupiedCells.value(extendedCell), 0, 0, 1, 3);
                addResizeButton(2, 1);
            }

            addResizeButton(1, 0);
            addResizeButton(1, 2);

            m_pGridLayout->setColumnStretch(0,1);
            m_pGridLayout->setColumnStretch(2,1);
        }

        QPoint shortenedCellIndex = indexForCell(shortenedCell);
        m_pGridLayout->addWidget(m_OccupiedCells.value(shortenedCell), shortenedCellIndex.y(), shortenedCellIndex.x());

        m_pGridLayout->addWidget(m_OccupiedCells.value(m_iLastCell), latestIndex.y(), latestIndex.x());
        addResizeButton(1, 1);
    }

    void ResizeableGridLayoutManager::setQuadItemLayout()
    {
        Q_ASSERT_X(m_OccupiedCells.count() == 4, Q_FUNC_INFO, "Expected four widgets!");

        for ( QHash<ContentCell, QWidget*>::const_iterator it = m_OccupiedCells.constBegin();
              it != m_OccupiedCells.constEnd();
              ++it)
        {
            QPoint index = indexForCell(it.key());
            m_pGridLayout->addWidget(it.value(), index.y(), index.x());
        }

        addResizeButton(0,1);
        addResizeButton(1,0);
        addResizeButton(1,1);
        addResizeButton(1,2);
        addResizeButton(2,1);
    }

    bool ResizeableGridLayoutManager::calculateDualItemSplit(ContentCell cell1, ContentCell cell2, ContentCell &lower, ContentCell &upper)
    {
        // The orientation of the two cells depends on the cells that are occupied.
        // If one cell is on the right and the other on the left (and the cells aren't diagonal),
        // the split is vertical. If one cell is on top and the other is on the bottom
        // (and the cells aren't diagonal), the split is horizontal. If the cells are diagonal,
        // the split is vertical if the leftmost cell is on top, and horizontal if the
        // leftmost cell is on the bottom.

        typedef QPair<ContentCell, QPoint> IndexPair;
        IndexPair index1(cell1, indexForCell(cell1));
        IndexPair index2(cell2, indexForCell(cell2));

        // Diagonal
        if ( index1.second.x() != index2.second.x() && index1.second.y() != index2.second.y() )
        {
            // What's the leftmost item?
            IndexPair& left = index1.second.x() < index2.second.x() ? index1 : index2;
            IndexPair& right = index1.second.x() < index2.second.x() ? index2 : index1;

            if ( left.second.y() < right.second.y() )
            {
                // Vertical
                lower = left.first;
                upper = right.first;
                return true;
            }
            else
            {
                // Horizontal
                lower = right.first;
                upper = left.first;
                return false;
            }
        }
        // Not diagonal
        else
        {
            // Which is the differing axis?
            if ( index1.second.x() != index2.second.x() )
            {
                // Vertical
                lower = index1.second.x() < index2.second.x() ? index1.first : index2.first;
                upper = index1.second.x() < index2.second.x() ? index2.first : index1.first;
                return true;
            }
            else
            {
                // Horizontal
                lower = index1.second.y() < index2.second.y() ? index1.first : index2.first;
                upper = index1.second.y() < index2.second.y() ? index2.first : index1.first;
                return false;
            }
        }
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

    void ResizeableGridLayoutManager::setRowColMinSize()
    {
        m_pGridLayout->setRowMinimumHeight(0, 1);
        m_pGridLayout->setRowMinimumHeight(2, 1);
        m_pGridLayout->setColumnMinimumWidth(0, 1);
        m_pGridLayout->setColumnMinimumWidth(2, 1);
    }

    void ResizeableGridLayoutManager::setStretchFactors(int row0, int row2, int col0, int col2)
    {
        m_pGridLayout->setRowStretch(0, row0);
        m_pGridLayout->setRowStretch(2, row2);
        m_pGridLayout->setColumnStretch(0, col0);
        m_pGridLayout->setColumnStretch(2, col2);
    }
}
