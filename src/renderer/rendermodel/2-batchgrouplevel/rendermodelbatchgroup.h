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
#include <QPair>
#include "shaders/ishaderspec.h"

namespace NS_RENDERER
{
    class RenderModelBatchGroup
    {
    public:
        RenderModelBatchGroup(QOpenGLBuffer::UsagePattern usagePattern, const IShaderSpec* shaderSpec);
        ~RenderModelBatchGroup();

        QOpenGLBuffer::UsagePattern usagePattern() const;

        MatrixBatch* createMatrixBatch(const MatrixBatchKey &key);
        MatrixBatch* getMatrixBatch(const MatrixBatchKey &key) const;
        void removeMatrixBatch(const MatrixBatchKey &key);
        bool containsMatrixBatch(const MatrixBatchKey &key) const;
        void clearMatrixBatches();

    private:
        typedef QSharedPointer<OpenGLBatch> OpenGLBatchPointer;
        typedef QPair<OpenGLBatchPointer, int> OpenGLBatchPointerIndex;

        void setFull(const OpenGLBatchPointer &batch);
        void setWaiting(const OpenGLBatchPointer &batch);
        static MatrixBatch* dereferenceGlBatchIndex(const OpenGLBatchPointerIndex &index);
        OpenGLBatchPointer getNextWaitingGlBatch();

        QSet<OpenGLBatchPointer>  m_WaitingBatches; // Batches that have space for more matrices.
        QSet<OpenGLBatchPointer>  m_FullBatches;    // Batches that have no more space.

        QHash<MatrixBatchKey, OpenGLBatchPointerIndex>   m_MatrixBatchMap;

        const QOpenGLBuffer::UsagePattern m_iUsagePattern;
        const IShaderSpec* m_pShaderSpec;
    };
}

#endif // RENDERMODELBATCHGROUP_H
