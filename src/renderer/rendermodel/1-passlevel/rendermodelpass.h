#ifndef RENDERMODELPASS_H
#define RENDERMODELPASS_H

#include "renderer_global.h"
#include <QMultiHash>
#include "rendermodel/3-batchlevel/rendermodelbatchkey.h"
#include "rendermodel/3-batchlevel/rendermodelbatch.h"
#include "rendermodel/3-batchlevel/rendermodelbatchparams.h"
#include <QMap>
#include "functors/ishaderretrievalfunctor.h"
#include "functors/itextureretrievalfunctor.h"
#include "rendermodel/1-passlevel/rendermodelpassattributes.h"
#include "opengl/opengluniformbuffer.h"

class QOpenGLTexture;

/* TODO: Rewrite this class (and probably RenderModelBatch too).
 * We need to have the following behaviour:
 * - Each RenderModelBatchKey (ie. shader, texture, draw mode, draw width) maps to one RenderModelBatch.
 * - Within each batch, each unique object ID maps to one RenderModelBatchItem. This item is replaced if
 *   a call with the same unique object ID is made again.
 * - On batch upload, all batch items with the same matrix are combined into one batch object (ie. are
 *   assigned the same object ID). Therefore object IDs are not assigned until upload is called!
 *
 * FYI, "addressing" a RenderModelBatchItem is done globally in the following way:
 * - A "global key" can be thought of as a pass index, shader/texture IDs, draw mode/width, model-
 *   to-world matrix and unique object ID.
 * - The pass is selected using the pass ID.
 * - The batch within the pass is selected using the shader/texture/draw mode (ie. the RenderModelBatchKey).
 * - The RenderModelBatchItem is selected using the unique object ID.
 * - When uploading the matrix is used to group objects into OpenGL buffers.
 */

namespace NS_RENDERER
{
    class OpenGLShaderProgram;

    class RENDERERSHARED_EXPORT RenderModelPass
    {
    public:
        // Functors provide shader/texture pointers given an ID.
        RenderModelPass(IShaderRetrievalFunctor* shaderFunctor, ITextureRetrievalFunctor* textureFunctor);
        ~RenderModelPass();

        bool create();
        void destroy();

        void addItem(const RenderModelBatchKey &key, const RenderModelBatchParams &params,
                     QOpenGLBuffer::UsagePattern usagePattern = QOpenGLBuffer::DynamicDraw);
        void clear();

        void debugDraw();
        void debugUploadAll(QVector<float> &vertexData, QVector<float> &uniformData, QVector<quint32> &indexData);
        void debugUploadAll();

        void setAttributes(const RenderModelPassAttributes &attributes);

        QOpenGLBuffer::UsagePattern usagePattern() const;
        void setUsagePattern(QOpenGLBuffer::UsagePattern pattern);

        void upload();

    private:
        typedef QMultiHash<RenderModelBatchKey, RenderModelBatch*> RenderModelTable;
        typedef QMap<RenderModelBatchKey, char> KeyOrderingMap;

        void uploadUniformData();

        RenderModelTable    m_Table;
        KeyOrderingMap      m_KeyOrdering;

        IShaderRetrievalFunctor* const  m_pShaderFunctor;
        ITextureRetrievalFunctor* const m_pTextureFunctor;
        RenderModelPassAttributes m_Attributes;

        QOpenGLBuffer::UsagePattern m_iUsagePattern;
        OpenGLUniformBuffer    m_GlUniformBuffer;
        bool    m_bCreated;
    };
}

#endif // RENDERMODELPASS_H
