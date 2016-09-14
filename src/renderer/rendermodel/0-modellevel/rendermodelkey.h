#ifndef RENDERMODELKEY_H
#define RENDERMODELKEY_H

#include "renderer_global.h"
#include "rendermodel/1-passlevel/rendermodelpasskey.h"
#include "rendermodel/2-batchgrouplevel/rendermodelbatchgroupkey.h"
#include "rendermodel/3-batchlevel/matrixbatchkey.h"
#include "rendermodel/4-batchitemlevel/matrixbatchitemkey.h"

namespace NS_RENDERER
{
    class RenderModelKey
    {
    public:
        RenderModelKey(const RenderModelPassKey& passKey, const RenderModelBatchGroupKey& batchGroupKey,
                       const MatrixBatchKey& matrixBatchKey, const MatrixBatchItemKey& matrixBatchItemKey);

        const RenderModelPassKey& passKey() const;
        const RenderModelBatchGroupKey& batchGroupKey() const;
        const MatrixBatchKey& matrixBatchKey() const;
        const MatrixBatchItemKey matrixBatchItemKey() const;

    private:
        const RenderModelPassKey  m_PassKey;
        const RenderModelBatchGroupKey m_BatchGroupKey;
        const MatrixBatchKey m_MatrixBatchKey;
        const MatrixBatchItemKey m_MatrixBatchItemKey;
    };
}

#endif // RENDERMODELKEY_H
