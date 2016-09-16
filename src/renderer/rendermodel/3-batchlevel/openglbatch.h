#ifndef OPENGLBATCH_H
#define OPENGLBATCH_H

#include "renderer_global.h"
#include <QOpenGLBuffer>
#include "opengl/opengluniformbuffer.h"
#include "shaders/vertexformat.h"
#include "general/fixedindexpool.h"
#include "matrixbatch.h"

namespace NS_RENDERER
{
    class OpenGLBatch
    {
    public:
        OpenGLBatch(QOpenGLBuffer::UsagePattern usagePattern, const VertexFormat &format, int batchSize);
        ~OpenGLBatch();

        void create();
        void destroy();
        bool isCreated() const;
        void upload();

        const VertexFormat& vertexFormat() const;
        int batchSize() const;
        bool isSingleItemBatch() const;

        QOpenGLBuffer::UsagePattern usagePattern() const;

        int createMatrixBatch();
        void destroyMatrixBatch(int index);
        int matrixBatchCount() const;
        bool matrixBatchLimitReached() const;
        MatrixBatch* matrixBatchAt(int index) const;
        void clearMatrixBatches();

    private:
        static inline quint32 maskFromNumberOfBits(int numBits)
        {
            return (quint32)(~0) >> ((sizeof(quint32) * 8) - numBits);
        }

        static inline quint8 bitsRequired(int maxValue)
        {
            for ( quint32 i = 0; i < sizeof(quint32)*8; i++ )
            {
                if ( 1 << i >= maxValue )
                {
                    return i+1;
                }
            }

            return sizeof(quint32)*8;
        }

        void uploadVertices();
        void uploadIndices();
        void uploadUniforms();

        const QOpenGLBuffer::UsagePattern m_iUsagePattern;
        bool m_bCreated;
        const VertexFormat m_VertexFormat;
        const int m_iBatchSize;
        const quint32 m_iObjectIdMask;

        QOpenGLBuffer       m_VertexBuffer;
        QOpenGLBuffer       m_IndexBuffer;
        OpenGLUniformBuffer m_UniformBuffer;
        QList<MatrixBatch*> m_MatrixBatches;
    };
}

#endif // OPENGLBATCH_H
