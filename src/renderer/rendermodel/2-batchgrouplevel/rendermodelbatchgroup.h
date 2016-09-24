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
        RenderModelBatchGroup(const RenderModelBatchGroupKey &key, QOpenGLBuffer::UsagePattern usagePattern, const IShaderSpec* shaderSpec);
        ~RenderModelBatchGroup();

        QOpenGLBuffer::UsagePattern usagePattern() const;
        const RenderModelBatchGroupKey& key() const;

        MatrixBatch* createMatrixBatch(const MatrixBatchKey &key);
        MatrixBatch* getMatrixBatch(const MatrixBatchKey &key) const;
        void removeMatrixBatch(const MatrixBatchKey &key);
        bool containsMatrixBatch(const MatrixBatchKey &key) const;
        int matrixBatchCount() const;
        void clearMatrixBatches();

        void printDebugInfo() const;

        void drawAllBatches(QOpenGLShaderProgram* shaderProgram);

    private:
        typedef QSharedPointer<OpenGLBatch> OpenGLBatchPointer;
        typedef QPair<OpenGLBatchPointer, int> OpenGLBatchPointerIndex;

        void setFull(const OpenGLBatchPointer &batch);
        void setWaiting(const OpenGLBatchPointer &batch);
        static MatrixBatch* dereferenceGlBatchIndex(const OpenGLBatchPointerIndex &index);
        OpenGLBatchPointer getNextWaitingGlBatch();
        void draw(QSet<OpenGLBatchPointer> &batches, QOpenGLShaderProgram* shaderProgram);
        void ensureAllBatchesUploaded(QSet<OpenGLBatchPointer> &batches);

        QSet<OpenGLBatchPointer>  m_WaitingBatches; // Batches that have space for more matrices.
        QSet<OpenGLBatchPointer>  m_FullBatches;    // Batches that have no more space.

        QHash<MatrixBatchKey, OpenGLBatchPointerIndex>   m_MatrixBatchMap;

        const RenderModelBatchGroupKey m_Key;   // For convenience
        const QOpenGLBuffer::UsagePattern m_iUsagePattern;
        const IShaderSpec* m_pShaderSpec;
    };
}

#endif // RENDERMODELBATCHGROUP_H
