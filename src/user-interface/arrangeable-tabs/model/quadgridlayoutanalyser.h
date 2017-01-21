#ifndef QUADGRIDLAYOUTANALYSER_H
#define QUADGRIDLAYOUTANALYSER_H

#include "user-interface_global.h"
#include <QObject>
#include "quadgridlayoutdefs.h"
#include "quadgridlayoutpoint.h"
#include <QSet>

namespace UserInterface
{
    class QuadGridLayoutAnalyser : public QObject
    {
        Q_OBJECT
    public:
        // Expects an array of 4 QWidget* pointers.
        explicit QuadGridLayoutAnalyser(const QWidget* gridCells, QObject *parent = 0);

        QuadGridLayoutDefs::MajorSplit majorSplit() const;
        QuadGridLayoutDefs::MinorSplit minorSplit() const;

        const QSet<QWidget*>& widgets() const;

    signals:

    public slots:
        void analyseLayout();

    private:
        void clear();
        void calculateMajorMinorSplit();
        QuadGridLayoutDefs::MajorSplit calculateMajorSplitFrom(QuadGridLayoutDefs::GridCell cell) const;
        QuadGridLayoutDefs::MajorSplit calculate3WidgetMajorSplit() const;
        QuadGridLayoutDefs::MinorSplit calculateMinorSplit() const;

        const QWidget* m_pGridCells;

        QSet<QWidget*> m_Widgets;
        QuadGridLayoutDefs::MajorSplit m_iMajorSplit;
        QuadGridLayoutDefs::MinorSplit m_iMinorSplit;
    };
}

#endif // QUADGRIDLAYOUTANALYSER_H
