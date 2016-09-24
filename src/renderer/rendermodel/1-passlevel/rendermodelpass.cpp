#include "rendermodelpass.h"
#include <QtDebug>

namespace
{
    void changeShaderIfDifferent(NS_RENDERER::OpenGLShaderProgram* &origShader,
                                 NS_RENDERER::OpenGLShaderProgram* newShader)
    {
        if ( origShader == newShader )
            return;

        if ( origShader )
        {
            origShader->disableAttributeArrays();
            origShader->release();
        }

        if ( newShader )
        {
            newShader->bind();
            newShader->enableAttributeArrays();
        }

        origShader = newShader;
    }

    void changeTextureIfDifferent(NS_RENDERER::OpenGLTexturePointer &origTexture,
                                  const NS_RENDERER::OpenGLTexturePointer &newTexture)
    {
        if ( origTexture == newTexture )
            return;

        if ( !origTexture.isNull() )
        {
            origTexture->release();
        }

        if ( !newTexture.isNull() )
        {
            newTexture->bind(0);
        }

        origTexture = newTexture;
    }
}

namespace NS_RENDERER
{
    RenderModelPass::RenderModelPass(IShaderRetrievalFunctor* shaderFunctor, ITextureRetrievalFunctor* textureFunctor)
        : m_pShaderFunctor(shaderFunctor), m_pTextureFunctor(textureFunctor)
    {
        Q_ASSERT_X(m_pShaderFunctor, Q_FUNC_INFO, "Shader functor should not be null!");
        Q_ASSERT_X(m_pTextureFunctor, Q_FUNC_INFO, "Texture functor should not be null!");
    }

    RenderModelPass::~RenderModelPass()
    {
        clearBatchGroups();
    }

    RenderModelPass::RenderModelBatchGroupPointer RenderModelPass::createBatchGroup(const RenderModelBatchGroupKey &key, QOpenGLBuffer::UsagePattern usagePattern)
    {
        OpenGLShaderProgram* shaderProgram = (*m_pShaderFunctor)(key.shaderId());

        RenderModelBatchGroupPointer batchGroup = RenderModelBatchGroupPointer::create(key, usagePattern, shaderProgram);
        m_BatchGroups.insert(key, batchGroup);
        return batchGroup;
    }

    RenderModelPass::RenderModelBatchGroupPointer RenderModelPass::getBatchGroup(const RenderModelBatchGroupKey &key) const
    {
        return m_BatchGroups.value(key, RenderModelBatchGroupPointer());
    }

    void RenderModelPass::removeBatchGroup(const RenderModelBatchGroupKey &key)
    {
        m_BatchGroups.remove(key);
    }

    bool RenderModelPass::containsBatchGroup(const RenderModelBatchGroupKey &key) const
    {
        return m_BatchGroups.contains(key);
    }

    void RenderModelPass::clearBatchGroups()
    {
        m_BatchGroups.clear();
    }

    void RenderModelPass::printDebugInfo() const
    {
        qDebug() << "Batch groups:" << m_BatchGroups.count();
    }

    void RenderModelPass::setIfRequired(const RenderModelBatchGroupKey &key, OpenGLShaderProgram *&shaderProgram, OpenGLTexturePointer &texture)
    {
        OpenGLShaderProgram* newShaderProgram = (*m_pShaderFunctor)(key.shaderId());
        changeShaderIfDifferent(shaderProgram, newShaderProgram);

        OpenGLTexturePointer newTexture = (*m_pTextureFunctor)(key.textureId());
        changeTextureIfDifferent(texture, newTexture);
    }

    void RenderModelPass::drawAllBatchGroups()
    {
        OpenGLShaderProgram* currentShaderProgram = NULL;
        OpenGLTexturePointer currentTexture;

        foreach ( const RenderModelBatchGroupPointer &batchGroup, m_BatchGroups.values() )
        {
            setIfRequired(batchGroup->key(), currentShaderProgram, currentTexture);
            batchGroup->drawAllBatches(currentShaderProgram);
        }

        changeShaderIfDifferent(currentShaderProgram, NULL);
        changeTextureIfDifferent(currentTexture, OpenGLTexturePointer());
    }
}
