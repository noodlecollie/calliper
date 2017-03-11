#ifndef RENDERMODELPASS_H
#define RENDERMODELPASS_H

#include "renderer_global.h"
#include <QMap>
#include <QSharedPointer>
#include "renderer/rendermodel/2-batchgrouplevel/rendermodelbatchgroup.h"
#include "renderer/rendermodel/2-batchgrouplevel/rendermodelbatchgroupkey.h"
#include "renderer/functors/renderfunctorgroup.h"
#include <QOpenGLBuffer>

namespace Renderer
{
    class RenderModelPass
    {
    public:
        typedef QSharedPointer<RenderModelBatchGroup> RenderModelBatchGroupPointer;

        RenderModelPass(const RenderFunctorGroup& renderFunctors);
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
        void setIfRequired(const RenderModelBatchGroupKey &key, OpenGLShaderProgram* &shaderProgram, RenderMaterialPointer &material);
        void setTextureUnitMap(const QMap<ShaderDefs::TextureUnit, quint32>& map);
        void changeMaterialIfDifferent(Renderer::RenderMaterialPointer &origMaterial,
                                      const Renderer::RenderMaterialPointer &newMaterial);

        const RenderFunctorGroup m_RenderFunctors;

        QMap<RenderModelBatchGroupKey, RenderModelBatchGroupPointer>  m_BatchGroups;
        QMap<ShaderDefs::TextureUnit, quint32> m_TextureUnitMap;
    };
}

#endif // RENDERMODELPASS_H
