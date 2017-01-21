#include "quadgridlayoutanalyser.h"

namespace UserInterface
{
    QuadGridLayoutAnalyser::QuadGridLayoutAnalyser(const QWidget* gridCells, QObject* parent)
        : QObject(parent), m_pGridCells(gridCells)
    {
        clear();
    }

    void QuadGridLayoutAnalyser::analyseLayout()
    {
        clear();

        for ( int i = 0; i < 4; ++i )
        {
            if ( m_pGridCells[i] == nullptr )
                continue;

            m_Widgets.insert(m_pGridCells[i]);
        }

        calculateMajorMinorSplit();
    }

    void QuadGridLayoutAnalyser::clear()
    {
        m_Widgets.clear();
        m_iMajorSplit = QuadGridLayoutDefs::MajorNone;
        m_iMinorSplit = QuadGridLayoutDefs::MinorNone;
    }

    QuadGridLayoutDefs::MajorSplit QuadGridLayoutAnalyser::majorSplit() const
    {
        return m_iMajorSplit;
    }

    QuadGridLayoutDefs::MinorSplit QuadGridLayoutAnalyser::minorSplit() const
    {
        return m_iMinorSplit;
    }

    const QSet<QWidget*>& QuadGridLayoutAnalyser::widgets() const
    {
        return m_Widgets;
    }

    void QuadGridLayoutAnalyser::calculateMajorMinorSplit()
    {
        switch ( m_Widgets.count() )
        {
            case 0:
            case 1:
            {
                m_iMajorSplit = QuadGridLayoutDefs::MajorNone;
                m_iMinorSplit = QuadGridLayoutDefs::MinorNone;
                break;
            }

            case 2:
            {
                m_iMajorSplit = calculateMajorSplitFrom(QuadGridLayoutDefs::NorthWest);
                m_iMinorSplit = QuadGridLayoutDefs::MinorNone;
                break;
            }

            case 3:
            {
                m_iMajorSplit = calculate3WidgetMajorSplit();
                m_iMinorSplit = calculateMinorSplit();
                break;
            }

            case 4:
            {
                m_iMajorSplit = QuadGridLayoutDefs::MajorBoth;
                m_iMinorSplit = QuadGridLayoutDefs::MinorNone;
                break;
            }

            default:
            {
                Q_ASSERT(false);
                m_iMajorSplit = QuadGridLayoutDefs::MajorNone;
                m_iMinorSplit = QuadGridLayoutDefs::MinorNone;
                break;
            }
        }
    }

    QuadGridLayoutDefs::MajorSplit QuadGridLayoutAnalyser::calculateMajorSplitFrom(QuadGridLayoutDefs::GridCell cell) const
    {
        QuadGridLayoutPoint point(cell);
        QWidget* w = m_pGridCells[point.toArrayIndex()];
        QWidget* wNeighbour = m_pGridCells[point.neighbour(Qt::Horizontal).toArrayIndex()];

        if ( w == wNeighbour )
            return QuadGridLayoutDefs::MajorHorizontal;

        wNeighbour = m_pGridCells[point.neighbour(Qt::Vertical).toArrayIndex()];

        if ( w == wNeighbour )
            return QuadGridLayoutDefs::MajorVertical;

        return QuadGridLayoutDefs::MajorNone;
    }

    QuadGridLayoutDefs::MajorSplit QuadGridLayoutAnalyser::calculate3WidgetMajorSplit() const
    {
        QuadGridLayoutDefs::MajorSplit split = calculateMajorSplitFrom(QuadGridLayoutDefs::NorthWest);
        if ( split != QuadGridLayoutDefs::MajorNone )
            return split;

        split = calculateMajorSplitFrom(QuadGridLayoutDefs::SouthEast);
        if ( split != QuadGridLayoutDefs::MajorNone )
            return split;

        Q_ASSERT(false);
        return QuadGridLayoutDefs::MajorNone;
    }

    QuadGridLayoutDefs::MinorSplit QuadGridLayoutAnalyser::calculateMinorSplit() const
    {
        Qt::Orientation stackingDirection =
                m_iMajorSplit == QuadGridLayoutDefs::MajorHorizontal
                    ? Qt::Horizontal
                    : Qt::Vertical;

        QuadGridLayoutPoint point(QuadGridLayoutDefs::NorthWest);
        QWidget* w = m_pGridCells[point.toArrayIndex()];
        QWidget* wNeighbour = m_pGridCells[point.neighbour(stackingDirection).toArrayIndex()];

        if ( w != wNeighbour )
        {
            return stackingDirection == Qt::Horizontal ? QuadGridLayoutDefs::MinorNorth : QuadGridLayoutDefs::MinorWest;
        }

        point = QuadGridLayoutPoint(QuadGridLayoutDefs::SouthEast);
        w = m_pGridCells[point.toArrayIndex()];
        wNeighbour = m_pGridCells[point.neighbour(stackingDirection).toArrayIndex()];

        if ( w != wNeighbour )
        {
            return stackingDirection == Qt::Horizontal ? QuadGridLayoutDefs::MinorSouth : QuadGridLayoutDefs::MinorEast;
        }

        Q_ASSERT(false);
        return QuadGridLayoutDefs::MinorNone;
    }
}
