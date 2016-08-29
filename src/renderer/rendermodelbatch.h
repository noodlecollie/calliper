#ifndef RENDERMODELBATCH_H
#define RENDERMODELBATCH_H

#include "renderer_global.h"
#include <QObject>
#include <QOpenGLBuffer>
#include "rendermodelbatchparams.h"
#include "rendermodelbatchitem.h"

namespace NS_RENDERER
{
    class IShaderSpec;

    class RENDERERSHARED_EXPORT RenderModelBatch : public QObject
    {
        Q_OBJECT
    public:
        RenderModelBatch(QOpenGLBuffer::UsagePattern usagePattern, QObject* parent = 0);
        ~RenderModelBatch();

        static const int MAX_ITEMS;

        GLuint vaoHandle() const;
        bool create();
        void destroy();

        void addItem(const RenderModelBatchParams &params);
        int itemCount() const;

        const IShaderSpec* shaderSpec() const;
        void setShaderSpec(const IShaderSpec* spec);

private:
        static void copyInVertexData(float* &dest, const float* source, int floatCount);
        static void copyInIndexData(quint32* &dest, const quint32* source, int intCount);
        int maxComponentsFromVertexSpec() const;

        GLuint  m_iVAOID;
        QOpenGLBuffer::UsagePattern m_iUsagePattern;
        bool    m_bCreated;

        QOpenGLBuffer   m_GlVertexBuffer;
        QOpenGLBuffer   m_GlIndexBuffer;

        QVector<float>  m_VertexBuffer;
        QVector<quint32>  m_IndexBuffer;
        QList<NS_RENDERER::RenderModelBatchItem> m_Items;

        const IShaderSpec*  m_pShaderSpec;
    };
}

#endif // RENDERMODELBATCH_H
