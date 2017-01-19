#ifndef QUADGRIDLAYOUTINFO_H
#define QUADGRIDLAYOUTINFO_H

#include "user-interface_global.h"

namespace UserInterface
{
    class QuadGridLayoutInfo
    {
    public:
        enum MajorSplit
        {
            MajorNone,
            MajorHorizontal,
            MajorVertical,
            MajorBoth,
        };

        enum MinorSplit
        {
            MinorNone,
            MinorNorth,
            MinorSouth,
            MinorEast,
            MinorWest
        };

        QuadGridLayoutInfo();
        QuadGridLayoutInfo(MajorSplit sMajor, MinorSplit sMinor = MinorNone);

        MajorSplit majorSplit() const;
        void setMajorSplit(MajorSplit split);

        MinorSplit minorSplit() const;
        void setMinorSplit(MinorSplit split);

        void setMajorMinorSplit(MajorSplit majSplit, MinorSplit minSplit);

    private:
        MajorSplit m_iMajorSplit;
        MinorSplit m_iMinorSplit;
    };
}

#endif // QUADGRIDLAYOUTINFO_H
