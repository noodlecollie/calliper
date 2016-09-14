#ifndef RENDERMODEL_H
#define RENDERMODEL_H

#include "renderer_global.h"
#include "rendermodelpass.h"
#include <QList>
#include "geometry/geometrybuilder.h"
#include "irenderer.h"
#include <QHash>

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

        void clear();
        int passCount() const;
        int createPass();

        virtual void updateItem(quint64 objectUniqueId, const RenderModelInputParams &params, const GeometrySectionList &geometry) override;

    private:
        QList<RenderModelPass*> m_RenderPasses;
        IShaderRetrievalFunctor* m_pShaderFunctor;
        ITextureRetrievalFunctor* m_pTextureFunctor;

        QHash<quint64, RenderModelInputParams> m_ItemTable;
    };
}

#endif // RENDERMODEL_H
