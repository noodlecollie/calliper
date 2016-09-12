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

class QOpenGLTexture;

namespace NS_RENDERER
{
    class OpenGLShaderProgram;

    class RENDERERSHARED_EXPORT RenderModelPass
    {
    public:
        RenderModelPass(IShaderRetrievalFunctor* shaderFunctor, ITextureRetrievalFunctor* textureFunctor);
        ~RenderModelPass();

        void addItem(const RenderModelBatchKey &key, const RenderModelBatchParams &params,
                     QOpenGLBuffer::UsagePattern usagePattern = QOpenGLBuffer::DynamicDraw);
        void clear();

        void debugDraw();
        void debugUploadAll(QVector<float> &vertexData, QVector<float> &uniformData, QVector<quint32> &indexData);
        void debugUploadAll();

    private:
        typedef QMultiHash<RenderModelBatchKey, RenderModelBatch*> RenderModelTable;
        typedef QMap<RenderModelBatchKey, char> KeyOrderingMap;

        RenderModelTable    m_Table;
        KeyOrderingMap      m_KeyOrdering;

        IShaderRetrievalFunctor* const  m_pShaderFunctor;
        ITextureRetrievalFunctor* const m_pTextureFunctor;
    };
}

#endif // RENDERMODELPASS_H
