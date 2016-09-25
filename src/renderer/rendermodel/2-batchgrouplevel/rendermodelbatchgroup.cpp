#include "rendermodelbatchgroup.h"
#include <QtDebug>
#include "opengl/openglhelpers.h"

namespace NS_RENDERER
{
    RenderModelBatchGroup::RenderModelBatchGroup(const RenderModelBatchGroupKey &key,
                                                 QOpenGLBuffer::UsagePattern usagePattern,
                                                 const IShaderSpec* shaderSpec)
        : m_Key(key), m_iUsagePattern(usagePattern), m_pShaderSpec(shaderSpec)
    {
    }

    RenderModelBatchGroup::~RenderModelBatchGroup()
    {
        clearMatrixBatches();
    }

    MatrixBatch* RenderModelBatchGroup::createMatrixBatch(const MatrixBatchKey &key)
    {
        // Get the GL batch that the matrix batch lives in.
        OpenGLBatchPointerIndex batchIndex = m_MatrixBatchMap.value(key, OpenGLBatchPointerIndex(OpenGLBatchPointer(), -1));

        // If we got a valid batch, don't bother removing and re-adding.
        // Just clear it out and return it.
        if ( !batchIndex.first.isNull() )
        {
            MatrixBatch* matBatch = dereferenceGlBatchIndex(batchIndex);
            matBatch->clearItems();
            return matBatch;
        }

        // If not, this means the key was not present in the map.
        // In this case we need to create a new MatrixBatch in an appropriate OpenGL batch.
        OpenGLBatchPointer glBatch = getNextWaitingGlBatch();
        Q_ASSERT_X(!glBatch->matrixBatchLimitReached(), Q_FUNC_INFO, "OpenGLBatch returned did not have space for a new MatrixBatch!");

        // Create the MatrixBatch and keep track of its index.
        int matrixBatchIndex = glBatch->createMatrixBatch(key.modelToWorldMatrix());

        // Move the OpenGL batch out of the waiting set if we need to.
        if ( glBatch->matrixBatchLimitReached() )
        {
            setFull(glBatch);
            Q_ASSERT(m_FullBatches.contains(glBatch));
            Q_ASSERT(!m_WaitingBatches.contains(glBatch));
        }

        // Store this mapping for later use.
        m_MatrixBatchMap.insert(key, OpenGLBatchPointerIndex(glBatch, matrixBatchIndex));

        // Return the actual MatrixBatch.
        return glBatch->matrixBatchAt(matrixBatchIndex);
    }

    void RenderModelBatchGroup::removeMatrixBatch(const MatrixBatchKey &key)
    {
        // If this key isn't stored by us, forget it.
        if ( !m_MatrixBatchMap.contains(key) )
            return;

        // Destroy the matrix batch.
        OpenGLBatchPointerIndex batchIndex = m_MatrixBatchMap.take(key);
        OpenGLBatchPointer& glBatch = batchIndex.first;
        glBatch->destroyMatrixBatch(batchIndex.second);

        // Update the waiting/full sets.
        if ( m_FullBatches.contains(glBatch) )
        {
            setWaiting(glBatch);
            Q_ASSERT(!m_FullBatches.contains(glBatch));
            Q_ASSERT(m_WaitingBatches.contains(glBatch));
        }

        // If the GlBatch is now completely empty, destroy it.
        // This would be called on destruction, but it's probably
        // safer to explicitly do it ourselves.
        if ( glBatch->matrixBatchCount() < 1 )
        {
            m_WaitingBatches.remove(glBatch);
            Q_ASSERT(!m_FullBatches.contains(glBatch));
            Q_ASSERT(!m_WaitingBatches.contains(glBatch));
            glBatch->destroy();
        }
    }

    bool RenderModelBatchGroup::containsMatrixBatch(const MatrixBatchKey &key) const
    {
        return m_MatrixBatchMap.contains(key);
    }

    MatrixBatch* RenderModelBatchGroup::getMatrixBatch(const MatrixBatchKey &key) const
    {
        OpenGLBatchPointerIndex batchIndex = m_MatrixBatchMap.value(key, OpenGLBatchPointerIndex(OpenGLBatchPointer(), -1));
        if ( batchIndex.first.isNull() )
            return NULL;

        return dereferenceGlBatchIndex(batchIndex);
    }

    void RenderModelBatchGroup::clearMatrixBatches()
    {
        // All the QSharedPointers should take care of the destruction.
        m_MatrixBatchMap.clear();
        m_WaitingBatches.clear();
        m_FullBatches.clear();
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

    MatrixBatch* RenderModelBatchGroup::dereferenceGlBatchIndex(const OpenGLBatchPointerIndex &index)
    {
        return index.first->matrixBatchAt(index.second);
    }

    RenderModelBatchGroup::OpenGLBatchPointer RenderModelBatchGroup::getNextWaitingGlBatch()
    {
        if ( m_WaitingBatches.isEmpty() )
        {
            OpenGLBatchPointer glBatch = OpenGLBatchPointer::create(m_iUsagePattern, m_pShaderSpec);
            glBatch->create();
            m_WaitingBatches.insert(glBatch);
        }

        return *m_WaitingBatches.begin();
    }

    void RenderModelBatchGroup::printDebugInfo() const
    {
        qDebug() << "Matrix batches:" << m_MatrixBatchMap.count()
                 << "Full OpenGL batches:" << m_FullBatches.count()
                 << "Waiting OpenGL batches:" << m_WaitingBatches.count();
    }

    int RenderModelBatchGroup::matrixBatchCount() const
    {
        return m_MatrixBatchMap.count();
    }

    void RenderModelBatchGroup::drawAllBatches(QOpenGLShaderProgram *shaderProgram)
    {
        ensureAllBatchesUploaded(m_FullBatches);
        ensureAllBatchesUploaded(m_WaitingBatches);

        draw(m_FullBatches, shaderProgram);
        draw(m_WaitingBatches, shaderProgram);
    }

    void RenderModelBatchGroup::draw(QSet<OpenGLBatchPointer> &batches, QOpenGLShaderProgram* shaderProgram)
    {
        typedef QSet<OpenGLBatchPointer> BatchSet;

        for ( BatchSet::iterator it = batches.begin(); it != batches.end(); ++it )
        {
            OpenGLBatchPointer batch = *it;
            batch->bindAll();
            batch->setVertexAttributes(shaderProgram);
            batch->draw();
            batch->releaseAll();
        }
    }

    void RenderModelBatchGroup::ensureAllBatchesUploaded(QSet<OpenGLBatchPointer> &batches)
    {
        typedef QSet<OpenGLBatchPointer> BatchSet;

        for ( BatchSet::iterator it = batches.begin(); it != batches.end(); ++it )
        {
            (*it)->uploadIfRequired();
        }
    }

    const RenderModelBatchGroupKey& RenderModelBatchGroup::key() const
    {
        return m_Key;
    }
}
