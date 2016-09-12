#ifndef RENDERMODELPASS_H
#define RENDERMODELPASS_H

#include "renderer_global.h"
#include <QMultiHash>
#include "rendermodelbatchkey.h"
#include "rendermodelbatch.h"
#include "rendermodelbatchparams.h"
#include <QMap>
#include "functors/ishaderretrievalfunctor.h"
#include "functors/itextureretrievalfunctor.h"
#include "rendermodel/rendermodelpassattributes.h"
#include "opengl/opengluniformbuffer.h"

class QOpenGLTexture;

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
