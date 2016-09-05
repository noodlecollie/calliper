#ifndef RENDERMODELBATCH_H
#define RENDERMODELBATCH_H

#include "renderer_global.h"
#include <QOpenGLBuffer>
#include "rendermodelbatchparams.h"
#include "rendermodelbatchitem.h"
#include "shaders/shaderdefs.h"
#include "opengl/opengluniformbuffer.h"
#include "shaders/vertexformat.h"

class QOpenGLShaderProgram;

namespace NS_RENDERER
{
    class IShaderSpec;

    class RenderModelBatch
    {
    public:
        RenderModelBatch(QOpenGLBuffer::UsagePattern usagePattern, const VertexFormat &vertexFormat, int batchSize);
        ~RenderModelBatch();

        bool create();
        void destroy();

        bool supportsBatching() const;

        // If this batch has reached max size and the data requires a new item, add() does nothing.
        void add(const RenderModelBatchParams &params);
        void clear();

        void upload(bool force);

private:
        static inline quint32 batchIdMask(int numBits)
        {
            return (quint32)(~0) >> ((sizeof(quint32) * 8) - numBits);
        }

        // TODO: This can probably be improved!
        static inline quint8 bitsRequired(int maxValue)
        {
            for ( int i = 0; i < sizeof(quint32)*8; i++ )
            {
                if ( 1 << i >= maxValue )
                {
                    return i+1;
                }
            }

            return sizeof(quint32)*8;
        }

        void uploadVertexData(const QVector<float> source, int &offsetBytes);
        int getVertexDataCountInBytes() const;
        int getIndexDataCountInBytes() const;
        void uploadAllVertexData();
        void uploadAllIndexData();

        QOpenGLBuffer::UsagePattern m_iUsagePattern;
        bool                        m_bCreated;

        QOpenGLBuffer   m_GlVertexBuffer;
        QOpenGLBuffer   m_GlIndexBuffer;
        OpenGLUniformBuffer m_GlUniformBuffer;

        VertexFormat            m_VertexFormat;
        int                     m_iBatchSize;
        bool                    m_bDataStale;
        const quint32           m_iBatchIdMask;
        GLuint                  m_iUniformBlockIndex;

        QHash<QMatrix4x4, RenderModelBatchItem*> m_ItemTable;
    };
}

#endif // RENDERMODELBATCH_H
