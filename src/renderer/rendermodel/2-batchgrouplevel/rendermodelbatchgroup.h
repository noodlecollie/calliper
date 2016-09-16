#ifndef RENDERMODELBATCHGROUP_H
#define RENDERMODELBATCHGROUP_H

#include "renderer_global.h"
#include <QSet>
#include "rendermodel/3-batchlevel/openglbatch.h"
#include <QHash>
#include "rendermodel/3-batchlevel/matrixbatchkey.h"
#include "rendermodel/3-batchlevel/matrixbatch.h"
#include <QOpenGLBuffer>
#include "shaders/vertexformat.h"

namespace NS_RENDERER
{
    class RenderModelBatchGroup
    {
    public:
        typedef QSharedPointer<MatrixBatch> MatrixBatchPointer;

        RenderModelBatchGroup(QOpenGLBuffer::UsagePattern usagePattern, const VertexFormat &format, int batchSize);

        QOpenGLBuffer::UsagePattern usagePattern() const;

        MatrixBatchPointer createMatrixBatch(const MatrixBatchKey &key);
        MatrixBatchPointer getMatrixBatch(const MatrixBatchKey &key);
        void removeMatrixBatch(const MatrixBatchKey &key);

    private:
        typedef QSharedPointer<OpenGLBatch> OpenGLBatchPointer;

        void createMatrixToGLBatchMapping(const MatrixBatchKey &key);
        void setFull(const OpenGLBatchPointer &batch);
        void setWaiting(const OpenGLBatchPointer &batch);

        QSet<OpenGLBatchPointer>  m_WaitingBatches; // Batches that have space for more matrices.
        QSet<OpenGLBatchPointer>  m_FullBatches;    // Batches that have no more space.

        QHash<MatrixBatchKey, OpenGLBatchPointer>   m_MatrixBatchMap;

        const QOpenGLBuffer::UsagePattern m_iUsagePattern;
        const VertexFormat m_VertexFormat;
        const int m_iBatchSize;
    };
}

#endif // RENDERMODELBATCHGROUP_H
