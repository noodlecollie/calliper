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
        clear();
    }

    RenderModelBatchGroup::MatrixBatchPointer RenderModelBatchGroup::createMatrixBatch(const MatrixBatchKey &key)
    {
        // Try and get the matrix batch for this key.
        MatrixBatchPointer matrixBatch = m_MatrixBatches.value(key, MatrixBatchPointer());

        // If it exists already, just clear it out.
        if ( !matrixBatch.isNull() )
        {
            matrixBatch->clearItems();
        }
        // Otherwise, we need to create it.
        else
        {
            matrixBatch = MatrixBatchPointer::create(key.modelToWorldMatrix());
            m_MatrixBatches.insert(key, matrixBatch);
        }

        // Check to see whether the matrix batch has been assigned an OpenGL batch.
        // We do this regardless of whether we just had to create a new batch.
        // If we didn't have to create one, it's still possible that it didn't belong
        // to an OpenGL batch (it could have been set as hidden).
        if ( !m_MatrixOpenGLMap.contains(key) )
        {
            // Get the next suitable OpenGL batch.
            OpenGLBatchPointer glBatch = getNextWaitingGlBatch();
            Q_ASSERT_X(!glBatch->matrixBatchLimitReached(), Q_FUNC_INFO,
                       "OpenGLBatch returned did not have space for a new MatrixBatch!");

            // Add the matrix batch.
            glBatch->insertMatrixBatch(key, matrixBatch);

            // Move the OpenGL batch out of the waiting set if we need to.
            if ( glBatch->matrixBatchLimitReached() )
            {
                setFull(glBatch);
                Q_ASSERT(m_FullBatches.contains(glBatch));
                Q_ASSERT(!m_WaitingBatches.contains(glBatch));
            }

            // Make sure we've recorded this mapping.
            m_MatrixOpenGLMap.insert(key, glBatch);
        }

        return matrixBatch;
    }

    void RenderModelBatchGroup::removeMatrixBatch(const MatrixBatchKey &key)
    {
        // If this key isn't stored by us, forget it.
        if ( !m_MatrixBatches.contains(key) )
            return;

        // See if it belongs to an OpenGL batch and remove it if so.
        OpenGLBatchPointer glBatch = m_MatrixOpenGLMap.value(key, OpenGLBatchPointer());
        if ( !glBatch.isNull() )
        {
            glBatch->removeMatrixBatch(key);

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
    }

    bool RenderModelBatchGroup::containsMatrixBatch(const MatrixBatchKey &key) const
    {
        return m_MatrixBatches.contains(key);
    }

    RenderModelBatchGroup::MatrixBatchPointer RenderModelBatchGroup::getMatrixBatch(const MatrixBatchKey &key) const
    {
        return m_MatrixBatches.value(key, MatrixBatchPointer());
    }

    void RenderModelBatchGroup::clear()
    {
        m_WaitingBatches.clear();
        m_FullBatches.clear();
        m_MatrixOpenGLMap.clear();
        m_MatrixBatches.clear();
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
        qDebug() << "Matrix batches:" << m_MatrixBatches.count()
                 << "Full OpenGL batches:" << m_FullBatches.count()
                 << "Waiting OpenGL batches:" << m_WaitingBatches.count()
                 << "Matrix batches in use:" << m_MatrixOpenGLMap.count();
    }

    int RenderModelBatchGroup::matrixBatchCount() const
    {
        return m_MatrixBatches.count();
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

    bool RenderModelBatchGroup::matrixBatchIsMemberOfOpenGLBatch(const MatrixBatchKey &key) const
    {
        return m_MatrixOpenGLMap.contains(key);
    }
}
