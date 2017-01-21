#ifndef QUADGRIDLAYOUTDEFS_H
#define QUADGRIDLAYOUTDEFS_H

#include "user-interface_global.h"
#include <QObject>

namespace UserInterface
{
    class QuadGridLayoutDefs
    {
        Q_GADGET
    public:
        enum GridCell
        {
            NorthWest = 0,
            NorthEast,
            SouthWest,
            SouthEast,
        };
        Q_ENUM(GridCell)

        enum MajorSplit
        {
            MajorNone = 0,
            MajorHorizontal,
            MajorVertical,
            MajorBoth,
        };
        Q_ENUM(MajorSplit)

        enum MinorSplit
        {
            MinorNone = 0,
            MinorNorth,
            MinorEast,
            MinorSouth,
            MinorWest,
        };
        Q_ENUM(MinorSplit)

        enum WidgetSpan
        {
            NoSpan = 0,
            HorizontalSpan,
            VerticalSpan,
        };
        Q_ENUM(WidgetSpan)

        enum WidgetOrdering
        {
            NewWidgetInLowerIndex = 0,
            NewWidgetInUpperIndex,
        };
        Q_ENUM(WidgetOrdering)

    private:
        QuadGridLayoutDefs() = delete;
    };
}

#endif // QUADGRIDLAYOUTDEFS_H
