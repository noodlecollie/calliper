#include "quadgridlayoutanalyser.h"
#include "quadgridlayoutmodel.h"

namespace UserInterface
{
    QuadGridLayoutAnalyser::QuadGridLayoutAnalyser(QuadGridLayoutModel *parentModel)
        : QObject(parentModel), m_pModel(parentModel)
    {
        Q_ASSERT(m_pModel);

        clear();
        connect(model(), &QuadGridLayoutModel::layoutUpdated, this, &QuadGridLayoutAnalyser::analyseLayout);
    }

    QuadGridLayoutModel* QuadGridLayoutAnalyser::model() const
    {
        return m_pModel;
    }

    void QuadGridLayoutAnalyser::analyseLayout()
    {
        clear();
        calculateMajorMinorSplit();
    }

    void QuadGridLayoutAnalyser::clear()
    {
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

    void QuadGridLayoutAnalyser::calculateMajorMinorSplit()
    {
        switch ( m_pModel->widgetCount() )
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
        QWidget* w = m_pModel->widgetAt(point);
        QWidget* wNeighbour = m_pModel->widgetAt(point.neighbour(Qt::Horizontal));

        if ( w == wNeighbour )
            return QuadGridLayoutDefs::MajorHorizontal;

        wNeighbour = m_pModel->widgetAt(point.neighbour(Qt::Vertical));

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
        QWidget* w = m_pModel->widgetAt(point);
        QWidget* wNeighbour = m_pModel->widgetAt(point.neighbour(stackingDirection));

        if ( w != wNeighbour )
        {
            return stackingDirection == Qt::Horizontal ? QuadGridLayoutDefs::MinorNorth : QuadGridLayoutDefs::MinorWest;
        }

        point = QuadGridLayoutPoint(QuadGridLayoutDefs::SouthEast);
        w = m_pModel->widgetAt(point);
        wNeighbour = m_pModel->widgetAt(point.neighbour(stackingDirection));

        if ( w != wNeighbour )
        {
            return stackingDirection == Qt::Horizontal ? QuadGridLayoutDefs::MinorSouth : QuadGridLayoutDefs::MinorEast;
        }

        Q_ASSERT(false);
        return QuadGridLayoutDefs::MinorNone;
    }
}
