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

        UniformBatchOffsets(quint32 offset, quint32 size)
            : batchOffsetBytes(offset),
              batchSizeBytes(size)
        {
        }

        UniformBatchOffsets::UniformBatchOffsets()
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
