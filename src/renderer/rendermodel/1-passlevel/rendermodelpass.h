#ifndef RENDERMODELPASS_H
#define RENDERMODELPASS_H

#include "renderer_global.h"
#include <QMap>
#include <QSharedPointer>
#include "rendermodel/2-batchgrouplevel/rendermodelbatchgroup.h"
#include "rendermodel/2-batchgrouplevel/rendermodelbatchgroupkey.h"
#include "functors/ishaderretrievalfunctor.h"
#include "functors/itextureretrievalfunctor.h"
#include "functors/imaterialretrievalfunctor.h"
#include <QOpenGLBuffer>

namespace Renderer
{
    class RenderModelPass
    {
    public:
        typedef QSharedPointer<RenderModelBatchGroup> RenderModelBatchGroupPointer;

        RenderModelPass(IShaderRetrievalFunctor* shaderFunctor, ITextureRetrievalFunctor* textureFunctor, IMaterialRetrievalFunctor* materialFunctor);
        ~RenderModelPass();

        RenderModelBatchGroupPointer createBatchGroup(const RenderModelBatchGroupKey &key,
                                                      QOpenGLBuffer::UsagePattern usagePattern = QOpenGLBuffer::DynamicDraw);
        RenderModelBatchGroupPointer getBatchGroup(const RenderModelBatchGroupKey &key) const;
        void removeBatchGroup(const RenderModelBatchGroupKey &key);
        bool containsBatchGroup(const RenderModelBatchGroupKey &key) const;
        void clearBatchGroups();

        void printDebugInfo() const;

        void drawAllBatchGroups(quint32 defaultTextureId);

    private:
        void setIfRequired(const RenderModelBatchGroupKey &key, OpenGLShaderProgram* &shaderProgram, RenderMaterialPointer &material);
        void setTextureUnitMap(const QMap<ShaderDefs::TextureUnit, quint32>& map);
        void changeMaterialIfDifferent(Renderer::RenderMaterialPointer &origMaterial,
                                      const Renderer::RenderMaterialPointer &newMaterial);

        IShaderRetrievalFunctor* const  m_pShaderFunctor;
        ITextureRetrievalFunctor* const m_pTextureFunctor;
        IMaterialRetrievalFunctor* const m_pMaterialFunctor;

        quint32 m_iDrawDefaultTextureId;
        QMap<RenderModelBatchGroupKey, RenderModelBatchGroupPointer>  m_BatchGroups;
        QMap<ShaderDefs::TextureUnit, quint32> m_TextureUnitMap;
    };
}

#endif // RENDERMODELPASS_H
