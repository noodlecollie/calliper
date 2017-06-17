#ifndef BATCHGENERATOR_H
#define BATCHGENERATOR_H

#include <QVector>

#include "geometrydatacontainer.h"

#include "rendersystem/private/rendermodel/rendermodelcontext.h"
#include "rendersystem/private/shaders/base/openglshaderprogram.h"

class BatchGenerator
{
public:
    typedef GeometryDataContainer::GeometryDataPointer GeometryDataPointer;
    typedef QVector<GeometryDataPointer> GeometryDataVector;

    BatchGenerator(const RenderModelContext& context,
                   const GeometryDataContainer& data);

    void buildBatches(int maxItemsPerBatch);
    const GeometryDataVector& batch(int index) const;
    int batchCount() const;

private:
    GeometryDataVector& nextCompatibleBatch(const GeometryDataKey& key);

    const RenderModelContext& m_Context;
    const GeometryDataContainer& m_Data;

    QVector<GeometryDataVector> m_Batches;
    int m_nMaxBatchedItems;

    GeometryDataContainer::ConstIterator m_itLastProcessedItem;
};

#endif // BATCHGENERATOR_H
