#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include "renderer_global.h"
#include "irenderer.h"
#include <QMap>
#include "rendermodel/1-passlevel/rendermodelpass.h"
#include "rendermodel/1-passlevel/rendermodelpasskey.h"
#include <QHash>
#include <QList>
#include "opengl/opengluniformbuffer.h"

namespace NS_RENDERER
{
    class RenderModel : public IRenderer
    {
    public:
        RenderModel();
        ~RenderModel();

        virtual IShaderRetrievalFunctor* shaderFunctor() override;
        virtual void setShaderFunctor(IShaderRetrievalFunctor *functor) override;
        virtual ITextureRetrievalFunctor* textureFunctor() override;
        virtual void setTextureFunctor(ITextureRetrievalFunctor *functor) override;

        virtual void updateObject(const RendererInputObjectParams &object) override;
        virtual void removeObject(quint32 objectId) override;

        virtual void draw(const RendererDrawParams &params) override;

        void printDebugInfo() const;

    private:
        typedef QSharedPointer<RenderModelPass> RenderModelPassPointer;
        typedef QSharedPointer<QList<RenderModelKey> > RenderModelKeyListPointer;

        RenderModelPassPointer createRenderPass(const RenderModelPassKey &key);
        RenderModelPassPointer getRenderPass(const RenderModelPassKey &key) const;
        void removeRenderPass(const RenderModelPassKey &key);
        bool containsRenderPass(const RenderModelPassKey &key);
        void clearRenderPasses();
        void uploadGlobalUniformData();

        MatrixBatch::MatrixBatchItemPointer createOrFetchMatrixBatchItem(const RenderModelKey &key);
        void cleanMatrixBatchItem(const RenderModelKey &key);
        bool getModelItems(const RenderModelKey &key,
                           RenderModelPassPointer &pass,
                           RenderModelPass::RenderModelBatchGroupPointer &batchGroup,
                           MatrixBatch* &matrixBatch,
                           MatrixBatch::MatrixBatchItemPointer &batchItem) const;

        IShaderRetrievalFunctor*    m_pShaderFunctor;
        ITextureRetrievalFunctor*   m_pTextureFunctor;
        RendererDrawParams          m_DrawParams;

        OpenGLUniformBuffer         m_GlobalUniformBuffer;
        bool                        m_bUniformDataUploaded;

        QMap<RenderModelPassKey, RenderModelPassPointer>   m_RenderPasses;
        QHash<quint32, RenderModelKeyListPointer> m_StoredObjects;
    };
}

#endif // RENDERMODEL_H
