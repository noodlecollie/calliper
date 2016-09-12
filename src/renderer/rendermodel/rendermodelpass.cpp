#include "rendermodelpass.h"
#include "opengl/openglshaderprogram.h"

namespace NS_RENDERER
{
    RenderModelPass::RenderModelPass(IShaderRetrievalFunctor* shaderFunctor, ITextureRetrievalFunctor* textureFunctor)
        : m_pShaderFunctor(shaderFunctor), m_pTextureFunctor(textureFunctor)
    {
        Q_ASSERT_X(m_pShaderFunctor, Q_FUNC_INFO, "Shader functor is required!");
        Q_ASSERT_X(m_pTextureFunctor, Q_FUNC_INFO, "Texture functor is required!");
    }

    RenderModelPass::~RenderModelPass()
    {
        clear();
    }

    void RenderModelPass::addItem(const RenderModelBatchKey &key, const RenderModelBatchParams &params, QOpenGLBuffer::UsagePattern usagePattern)
    {
        OpenGLShaderProgram* program = (*m_pShaderFunctor)(key.shaderStoreId());
        Q_ASSERT_X(program, Q_FUNC_INFO, "Invalid shader specified in key!");

        RenderModelBatch* batch = NULL;
        if ( !m_Table.contains(key) || (batch = m_Table.find(key).value())->canAddNewItem(params.modelToWorldMatrix()) )
        {
            batch = new RenderModelBatch(usagePattern, program->vertexFormat(), program->maxBatchedItems());
            m_Table.insert(key, batch);
            m_KeyOrdering.insert(key, 0);
        }

        // This should never happen, but just to be safe:
        Q_ASSERT_X(batch, Q_FUNC_INFO, "No render batch pointer acquired!");

        batch->create();
        batch->add(params);
    }

    void RenderModelPass::clear()
    {
        qDeleteAll(m_Table.values());
        m_Table.clear();
        m_KeyOrdering.clear();
    }

    void RenderModelPass::debugDraw()
    {
        OpenGLShaderProgram* currentProgram = NULL;
        OpenGLTexturePointer currentTexture;
        foreach ( const RenderModelBatchKey &key, m_KeyOrdering.keys() )
        {
            RenderModelBatch* batch = m_Table.value(key);

            OpenGLShaderProgram* program = (*m_pShaderFunctor)(key.shaderStoreId());
            if ( program != currentProgram )
            {
                if ( currentProgram )
                {
                    currentProgram->disableAttributeArrays();
                    currentProgram->release();
                }

                currentProgram = program;
                currentProgram->bind();
                currentProgram->enableAttributeArrays();
            }

            OpenGLTexturePointer texture = (*m_pTextureFunctor)(key.textureId());
            if ( texture != currentTexture )
            {
                if ( currentTexture )
                {
                    currentTexture->release();
                }

                currentTexture = texture;
                currentTexture->bind(0);
            }

            batch->bindDraw();
            batch->setAttributePointers(program);
            batch->draw();
            batch->releaseDraw();
        }

        currentTexture->release();

        currentProgram->disableAttributeArrays();
        currentProgram->release();
    }

    void RenderModelPass::debugUploadAll(QVector<float> &vertexData, QVector<float> &uniformData, QVector<quint32> &indexData)
    {
        foreach ( RenderModelBatch* batch, m_Table.values() )
        {
            batch->upload(vertexData, uniformData, indexData);
        }
    }

    void RenderModelPass::debugUploadAll()
    {
        foreach ( RenderModelBatch* batch, m_Table.values() )
        {
            batch->upload();
        }
    }
}
