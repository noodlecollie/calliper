#include "rendermodelbatchgroup.h"

namespace NS_RENDERER
{
    RenderModelBatchGroup::RenderModelBatchGroup(QOpenGLBuffer::UsagePattern usagePattern, const VertexFormat &format, int batchSize)
        : m_iUsagePattern(usagePattern), m_VertexFormat(format), m_iBatchSize(batchSize)
    {

    }

    RenderModelBatchGroup::MatrixBatchPointer RenderModelBatchGroup::createMatrixBatch(const MatrixBatchKey &key)
    {
        if ( !m_MatrixBatchMap.contains(key) )
        {
            createMatrixToGLBatchMapping(key);
        }

        OpenGLBatchPointer glBatch = m_MatrixBatchMap.value(key);

        Q_ASSERT_X(!glBatch->isNull(), Q_FUNC_INFO, "Expected a non-null OpenGLBatch!");
        Q_ASSERT_X(!glBatch->matrixBatchLimitReached(), Q_FUNC_INFO, "Expected a free MatrixBatch slot in the OpenGLBatch!");
        Q_ASSERT_X(m_WaitingBatches.contains(glBatch), Q_FUNC_INFO, "Expected the batch returned to be in the waiting set!");
        Q_ASSERT_X(!m_FullBatches.contains(glBatch), Q_FUNC_INFO, "Expected the batch returned not to be in the full set!");

        MatrixBatchPointer matBatch = glBatch->createMatrixBatch();

        if ( glBatch->matrixBatchLimitReached() )
        {
            setFull(glBatch);
        }

        return matBatch;
    }

    void RenderModelBatchGroup::createMatrixToGLBatchMapping(const MatrixBatchKey &key)
    {
        if ( m_WaitingBatches.isEmpty() )
        {
            OpenGLBatchPointer glBatch(new OpenGLBatch(m_iUsagePattern, m_VertexFormat, m_iBatchSize));
            glBatch->create();
            m_WaitingBatches.insert(glBatch);
        }

        m_MatrixBatchMap.insert(key, *m_WaitingBatches.begin());
    }

    QOpenGLBuffer::UsagePattern RenderModelBatchGroup::usagePattern() const
    {
        return m_iUsagePattern;
    }

    void RenderModelBatchGroup::setFull(const OpenGLBatchPointer &batch)
    {
        m_WaitingBatches.remove(batch);
        m_FullBatches.insert(batch);
    }

    void RenderModelBatchGroup::setWaiting(const OpenGLBatchPointer &batch)
    {
        m_FullBatches.remove(batch);
        m_WaitingBatches.insert(batch);
    }

    RenderModelBatchGroup::MatrixBatchPointer RenderModelBatchGroup::getMatrixBatch(const MatrixBatchKey &key)
    {
        OpenGLBatchPointer glBatch = m_MatrixBatchMap.value(key, OpenGLBatchPointer(NULL));
        if ( glBatch.isNull() )
        {
            return MatrixBatchPointer(NULL);
        }
    }
}
