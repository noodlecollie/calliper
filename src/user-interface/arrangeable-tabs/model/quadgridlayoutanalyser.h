#ifndef QUADGRIDLAYOUTANALYSER_H
#define QUADGRIDLAYOUTANALYSER_H

#include "user-interface_global.h"
#include <QObject>
#include "quadgridlayoutdefs.h"
#include "quadgridlayoutpoint.h"
#include <QSet>

namespace UserInterface
{
    class QuadGridLayoutModel;

    class USERINTERFACESHARED_EXPORT QuadGridLayoutAnalyser : public QObject
    {
        Q_OBJECT
    public:
        explicit QuadGridLayoutAnalyser(QuadGridLayoutModel* parentModel);

        QuadGridLayoutModel* model() const;

        QuadGridLayoutDefs::MajorSplit majorSplit() const;
        QuadGridLayoutDefs::MinorSplit minorSplit() const;

    signals:

    public slots:
        void analyseLayout();

    private:
        void clear();
        void calculateMajorMinorSplit();
        QuadGridLayoutDefs::MajorSplit calculateMajorSplitFrom(QuadGridLayoutDefs::GridCell cell) const;
        QuadGridLayoutDefs::MajorSplit calculate3WidgetMajorSplit() const;
        QuadGridLayoutDefs::MinorSplit calculateMinorSplit() const;

        QuadGridLayoutModel* m_pModel;
        QuadGridLayoutDefs::MajorSplit m_iMajorSplit;
        QuadGridLayoutDefs::MinorSplit m_iMinorSplit;
    };
}

#endif // QUADGRIDLAYOUTANALYSER_H
