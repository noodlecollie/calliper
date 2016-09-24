#ifndef RENDERMODELPASS_H
#define RENDERMODELPASS_H

#include "renderer_global.h"
#include <QMap>
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

        RenderModelPass(IShaderRetrievalFunctor* shaderFunctor, ITextureRetrievalFunctor* textureFunctor);
        ~RenderModelPass();

        RenderModelBatchGroupPointer createBatchGroup(const RenderModelBatchGroupKey &key,
                                                      QOpenGLBuffer::UsagePattern usagePattern = QOpenGLBuffer::DynamicDraw);
        RenderModelBatchGroupPointer getBatchGroup(const RenderModelBatchGroupKey &key) const;
        void removeBatchGroup(const RenderModelBatchGroupKey &key);
        bool containsBatchGroup(const RenderModelBatchGroupKey &key) const;
        void clearBatchGroups();

        void printDebugInfo() const;

        void drawAllBatchGroups();

    private:
        void setIfRequired(const RenderModelBatchGroupKey &key, OpenGLShaderProgram* &shaderProgram, OpenGLTexturePointer &texture);

        IShaderRetrievalFunctor* const  m_pShaderFunctor;
        ITextureRetrievalFunctor* const m_pTextureFunctor;

        QMap<RenderModelBatchGroupKey, RenderModelBatchGroupPointer>  m_BatchGroups;
    };
}

#endif // RENDERMODELPASS_H
