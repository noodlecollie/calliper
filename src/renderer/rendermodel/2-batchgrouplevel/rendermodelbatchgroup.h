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
#include <QOpenGLShaderProgram>
#include "rendermodel/2-batchgrouplevel/rendermodelbatchgroupkey.h"

namespace NS_RENDERER
{
    class RenderModelBatchGroup
    {
    public:
        typedef QSharedPointer<MatrixBatch> MatrixBatchPointer;

        RenderModelBatchGroup(const RenderModelBatchGroupKey &key, QOpenGLBuffer::UsagePattern usagePattern, const IShaderSpec* shaderSpec);
        ~RenderModelBatchGroup();

        QOpenGLBuffer::UsagePattern usagePattern() const;
        const RenderModelBatchGroupKey& key() const;

        MatrixBatchPointer createMatrixBatch(const MatrixBatchKey &key);
        MatrixBatchPointer getMatrixBatch(const MatrixBatchKey &key) const;
        void removeMatrixBatch(const MatrixBatchKey &key);
        bool containsMatrixBatch(const MatrixBatchKey &key) const;
        int matrixBatchCount() const;
        bool matrixBatchIsMemberOfOpenGLBatch(const MatrixBatchKey &key) const;
        void clear();

        void printDebugInfo() const;

        void drawAllBatches(QOpenGLShaderProgram* shaderProgram);

    private:
        typedef QSharedPointer<OpenGLBatch> OpenGLBatchPointer;

        void setFull(const OpenGLBatchPointer &batch);
        void setWaiting(const OpenGLBatchPointer &batch);
        OpenGLBatchPointer getNextWaitingGlBatch();
        void draw(QSet<OpenGLBatchPointer> &batches, QOpenGLShaderProgram* shaderProgram);
        void ensureAllBatchesUploaded(QSet<OpenGLBatchPointer> &batches);

        QSet<OpenGLBatchPointer>  m_WaitingBatches; // Batches that have space for more matrices.
        QSet<OpenGLBatchPointer>  m_FullBatches;    // Batches that have no more space.

        QHash<MatrixBatchKey, MatrixBatchPointer> m_MatrixBatches;
        QHash<MatrixBatchKey, OpenGLBatchPointer> m_MatrixOpenGLMap;

        const RenderModelBatchGroupKey m_Key;   // For convenience
        const QOpenGLBuffer::UsagePattern m_iUsagePattern;
        const IShaderSpec* m_pShaderSpec;
    };
}

#endif // RENDERMODELBATCHGROUP_H
