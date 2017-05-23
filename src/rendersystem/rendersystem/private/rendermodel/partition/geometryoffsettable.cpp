#include "geometryoffsettable.h"

namespace
{
    GeometryOffsetTable::ObjectOffsets dummyItem;
}

GeometryOffsetTable::GeometryOffsetTable()
{

}

int GeometryOffsetTable::count() const
{
    return m_Offsets.count();
}

void GeometryOffsetTable::clear()
{
    m_Offsets.clear();
}

GeometryOffsetTable::ObjectOffsets GeometryOffsetTable::at(int index) const
{
    if ( index < 0 || index >= count() )
    {
        return ObjectOffsets();
    }

    return m_Offsets.at(index);
}

GeometryOffsetTable::ObjectOffsets& GeometryOffsetTable::operator [](int index)
{
    return m_Offsets[index];
}

const GeometryOffsetTable::ObjectOffsets& GeometryOffsetTable::operator [](int index) const
{
    return m_Offsets[index];
}

GeometryOffsetTable::ObjectOffsets& GeometryOffsetTable::createNewItem()
{
    m_Offsets.append(ObjectOffsets());
    return lastItem();
}

GeometryOffsetTable::ObjectOffsets& GeometryOffsetTable::lastItem()
{
    return m_Offsets.isEmpty() ? dummyItem : m_Offsets.last();
}
