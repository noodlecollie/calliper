#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include "renderer_global.h"
#include "renderer/irenderer.h"
#include <QMap>
#include "renderer/rendermodel/1-passlevel/rendermodelpass.h"
#include "renderer/rendermodel/1-passlevel/rendermodelpasskey.h"
#include <QHash>
#include <QList>
#include "renderer/shaders/globalshaderuniforms.h"
#include "renderer/opengl/openglvertexarrayobject.h"

namespace Renderer
{
    class RENDERERSHARED_EXPORT RenderModel : public IRenderer
    {
    public:
        RenderModel();
        ~RenderModel();

        virtual IShaderRetrievalFunctor* shaderFunctor() const override;
        virtual void setShaderFunctor(IShaderRetrievalFunctor *functor) override;
        virtual ITextureRetrievalFunctor* textureFunctor() const override;
        virtual void setTextureFunctor(ITextureRetrievalFunctor *functor) override;
        virtual IMaterialRetrievalFunctor* materialFunctor() const override;
        virtual void setMaterialFunctor(IMaterialRetrievalFunctor* functor) override;

        virtual void updateObject(const RendererInputObjectParams &object) override;
        virtual void removeObject(quint32 objectId) override;

        virtual void draw(const RendererDrawParams &params) override;

        virtual void setObjectFlags(quint32 objectId, quint32 flags) override;
        virtual void clearObjectFlags(quint32 objectId, quint32 flags) override;
        virtual quint32 getObjectFlags(quint32 objectId) const override;

        void printDebugInfo() const;

    private:
        typedef QSharedPointer<RenderModelPass> RenderModelPassPointer;
        typedef QSharedPointer<QList<RenderModelKey> > RenderModelKeyListPointer;

        RenderModelPassPointer createRenderPass(const RenderModelPassKey &key);
        RenderModelPassPointer getRenderPass(const RenderModelPassKey &key) const;
        void removeRenderPass(const RenderModelPassKey &key);
        bool containsRenderPass(const RenderModelPassKey &key);
        void clearRenderPasses();
        void uploadGlobalShaderUniforms();
        void updateGlobalShaderUniforms();
        void setObjectHidden(quint32 objectId, bool hidden);

        MatrixBatch::MatrixBatchItemPointer createOrFetchMatrixBatchItem(const RenderModelKey &key,
                                                RenderModelPass::RenderModelBatchGroupPointer* batchGroup = nullptr);
        void cleanMatrixBatchItem(const RenderModelKey &key);
        bool getModelItems(const RenderModelKey &key,
                           RenderModelPassPointer &pass,
                           RenderModelPass::RenderModelBatchGroupPointer &batchGroup,
                           RenderModelBatchGroup::MatrixBatchPointer &matrixBatch,
                           MatrixBatch::MatrixBatchItemPointer &batchItem) const;

        IShaderRetrievalFunctor*    m_pShaderFunctor;
        ITextureRetrievalFunctor*   m_pTextureFunctor;
        IMaterialRetrievalFunctor*  m_pMaterialFunctor;
        RendererDrawParams          m_DrawParams;

        GlobalShaderUniforms        m_GlobalShaderUniforms;
        OpenGLVertexArrayObject     m_VAO;

        QMap<RenderModelPassKey, RenderModelPassPointer>   m_RenderPasses;
        QHash<quint32, RenderModelKeyListPointer> m_StoredObjects;
        QHash<quint32, quint32> m_ObjectFlags;
    };
}

#endif // RENDERMODEL_H
