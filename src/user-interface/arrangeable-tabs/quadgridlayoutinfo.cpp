#include "quadgridlayoutinfo.h"

namespace UserInterface
{
    QuadGridLayoutInfo::QuadGridLayoutInfo()
        : m_iMajorSplit(MajorNone),
          m_iMinorSplit(MinorNone)
    {

    }

    QuadGridLayoutInfo::QuadGridLayoutInfo(MajorSplit sMajor, MinorSplit sMinor)
        : m_iMajorSplit(sMajor),
          m_iMinorSplit(sMinor)
    {

    }

    QuadGridLayoutInfo::MajorSplit QuadGridLayoutInfo::majorSplit() const
    {
        return m_iMajorSplit;
    }

    void QuadGridLayoutInfo::setMajorSplit(MajorSplit split)
    {
        m_iMajorSplit = split;
    }

    QuadGridLayoutInfo::MinorSplit QuadGridLayoutInfo::minorSplit() const
    {
        return m_iMinorSplit;
    }

    void QuadGridLayoutInfo::setMinorSplit(MinorSplit split)
    {
        m_iMinorSplit = split;
    }

    void QuadGridLayoutInfo::setMajorMinorSplit(MajorSplit majSplit, MinorSplit minSplit)
    {
        m_iMajorSplit = majSplit;
        m_iMinorSplit = minSplit;
    }
}
