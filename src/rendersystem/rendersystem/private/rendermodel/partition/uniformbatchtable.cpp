#include "uniformbatchtable.h"

namespace
{
    UniformBatchTable::UniformBatchOffsets dummyItem;
}

UniformBatchTable::UniformBatchTable()
    : m_Offsets()
{

}

int UniformBatchTable::count() const
{
    return m_Offsets.count();
}

UniformBatchTable::UniformBatchOffsets UniformBatchTable::at(int index) const
{
    return m_Offsets.at(index);
}

void UniformBatchTable::clear()
{
    m_Offsets.clear();
}

UniformBatchTable::UniformBatchOffsets& UniformBatchTable::operator [](int index)
{
    return m_Offsets[index];
}

const UniformBatchTable::UniformBatchOffsets& UniformBatchTable::operator [](int index) const
{
    return m_Offsets[index];
}

UniformBatchTable::UniformBatchOffsets& UniformBatchTable::createNewItem()
{
    m_Offsets.append(UniformBatchOffsets());
    return m_Offsets.last();
}

UniformBatchTable::UniformBatchOffsets& UniformBatchTable::lastItem()
{
    return m_Offsets.isEmpty() ? dummyItem : m_Offsets.last();
}
