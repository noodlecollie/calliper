#ifndef BUFFERDATACONTAINER_H
#define BUFFERDATACONTAINER_H

#include <QSharedPointer>
#include <QVector>

#include "rendersystem/private/rendermodel-removeme/raw-data/objectsectiongeometrydata.h"

class BufferDataContainer // TODO: Rename to something like GeometryBatchContainer
{
public:
    typedef QSharedPointer<ObjectSectionGeometryData> ObjectSectionGeometryDataPointer;

    BufferDataContainer(int numBatches, int numItemsPerBatch);

    int numBatches() const;
    int numItemsPerBatch() const;
    int maxBatchItems() const;

    // Number of batches that are at least partially full.
    int batchCount() const;

    int batchItemCount() const;

    bool isFull() const;
    bool isEmpty() const;

    bool addGeometryData(const ObjectSectionGeometryDataPointer& data);
    void clearItems();

    const ObjectSectionGeometryDataPointer& geometryData(int index) const;

private:
    const int m_nNumBatches;
    const int m_nNumItemsPerBatch;

    QVector<ObjectSectionGeometryDataPointer> m_Items;
};

#endif // BUFFERDATACONTAINER_H
