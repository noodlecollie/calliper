#ifndef UNIFORMBATCHTABLE_H
#define UNIFORMBATCHTABLE_H

#include <QVector>

class UniformBatchTable
{
public:
    struct UniformBatchOffsets
    {
        quint32 batchOffsetBytes;
        quint32 batchSizeBytes;
        quint32 firstGeometryItem;
        quint32 lastGeometryItem;

        UniformBatchOffsets(quint32 offset, quint32 size)
            : batchOffsetBytes(offset),
              batchSizeBytes(size),
              firstGeometryItem(0),
              lastGeometryItem(0)
        {
        }

        UniformBatchOffsets()
            : UniformBatchOffsets(0,0)
        {
        }
    };

    UniformBatchTable();

    int count() const;
    void clear();
    UniformBatchOffsets at(int index) const;

    UniformBatchOffsets& operator [](int index);
    const UniformBatchOffsets& operator [](int index) const;

    UniformBatchOffsets& createNewItem();
    UniformBatchOffsets& lastItem();

private:
    QVector<UniformBatchOffsets> m_Offsets;
};

#endif // UNIFORMBATCHTABLE_H
