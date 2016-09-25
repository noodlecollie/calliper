#ifndef OPENGLBATCH_H
#define OPENGLBATCH_H

#include "renderer_global.h"
#include <QOpenGLBuffer>
#include "opengl/opengluniformbuffer.h"
#include "shaders/vertexformat.h"
#include "general/fixedindexpool.h"
#include "matrixbatch.h"
#include "shaders/ishaderspec.h"
#include <QOpenGLShaderProgram>
#include "shaders/shaderdefs.h"

namespace NS_RENDERER
{
    class OpenGLBatch
    {
    public:
        OpenGLBatch(QOpenGLBuffer::UsagePattern usagePattern, const IShaderSpec* shaderSpec);
        ~OpenGLBatch();

        void create();
        void destroy();
        bool isCreated() const;
        void upload();

        const IShaderSpec* shaderSpec() const;
        VertexFormat vertexFormat() const;
        int batchSize() const;
        bool isSingleItemBatch() const;

        QOpenGLBuffer::UsagePattern usagePattern() const;

        int createMatrixBatch(const QMatrix4x4 &mat);
        void destroyMatrixBatch(int index);
        int matrixBatchCount() const;
        bool matrixBatchLimitReached() const;
        MatrixBatch* matrixBatchAt(int index) const;
        void clearMatrixBatches();

        // The state of this flag must be managed externally.
        // It's just for convenience.
        bool needsUpload() const;
        void setNeedsUpload(bool needsUpload);
        void uploadIfRequired();

        void setVertexAttributes(QOpenGLShaderProgram* shaderProgram) const;
        void bindAll();
        void draw();
        void releaseAll();

        void exportVertexData(QVector<float> &out);
        void exportIndexData(QVector<quint32> &out);

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
        void calculateRequiredSizeOfBuffers();
        void setAttributeBuffer(QOpenGLShaderProgram* shaderProgram,
                                NS_RENDERER::ShaderDefs::VertexArrayAttribute att,
                                int &offsetInBytes) const;
        void copyAllVertexData(char* buffer, int size);
        void updateObjectIds(char* buffer, int offset, int numBytes, quint32 id);

        const QOpenGLBuffer::UsagePattern m_iUsagePattern;
        bool m_bCreated;
        const IShaderSpec* m_pShaderSpec;
        const quint32 m_iMaskBitsRequired;
        const quint32 m_iObjectIdMask;

        QOpenGLBuffer       m_VertexBuffer;
        QOpenGLBuffer       m_IndexBuffer;
        OpenGLUniformBuffer m_UniformBuffer;
        bool                m_bNeedsUpload;
        QList<MatrixBatch*> m_MatrixBatches;
        MatrixBatchItemMetadata m_UploadMetadata;
    };
}

#endif // OPENGLBATCH_H
