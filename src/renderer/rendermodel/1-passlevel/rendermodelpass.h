#ifndef RENDERMODELPASS_H
#define RENDERMODELPASS_H

#include "renderer_global.h"
#include <QHash>
#include <QSharedPointer>
#include "rendermodel/2-batchgrouplevel/rendermodelbatchgroup.h"
#include "rendermodel/2-batchgrouplevel/rendermodelbatchgroupkey.h"
#include "functors/ishaderretrievalfunctor.h"
#include "functors/itextureretrievalfunctor.h"
#include <QOpenGLBuffer>

namespace NS_RENDERER
{
    class RenderModelPass
    {
    public:
        typedef QSharedPointer<RenderModelBatchGroup> RenderModelBatchGroupPointer;

        RenderModelPass(IShaderRetrievalFunctor* shaderFunctor);
        ~RenderModelPass();

        RenderModelBatchGroupPointer createBatchGroup(const RenderModelBatchGroupKey &key,
                                                      QOpenGLBuffer::UsagePattern usagePattern = QOpenGLBuffer::DynamicDraw);
        RenderModelBatchGroupPointer getBatchGroup(const RenderModelBatchGroupKey &key) const;
        void removeBatchGroup(const RenderModelBatchGroupKey &key);
        bool containsBatchGroup(const RenderModelBatchGroupKey &key) const;
        void clearBatchGroups();

    private:
        IShaderRetrievalFunctor* const  m_pShaderFunctor;

        QHash<RenderModelBatchGroupKey, RenderModelBatchGroupPointer>  m_BatchGroups;
    };
}

#endif // RENDERMODELPASS_H
