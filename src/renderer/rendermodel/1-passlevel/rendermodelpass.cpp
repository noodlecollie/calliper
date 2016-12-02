#include "rendermodelpass.h"
#include <QtDebug>
#include "opengl/openglhelpers.h"
#include "opengl/openglerrors.h"
#include "shaders/shaderdefs.h"

namespace
{
    void changeShaderIfDifferent(Renderer::OpenGLShaderProgram* &origShader,
                                 Renderer::OpenGLShaderProgram* newShader)
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
            newShader->setGlobalUniformBlockBinding();
            newShader->setLocalUniformBlockBinding();
            newShader->enableAttributeArrays();
        }

        origShader = newShader;
    }
}

namespace Renderer
{
    RenderModelPass::RenderModelPass(IShaderRetrievalFunctor* shaderFunctor, ITextureRetrievalFunctor* textureFunctor, IMaterialRetrievalFunctor* materialFunctor)
        : m_pShaderFunctor(shaderFunctor), m_pTextureFunctor(textureFunctor), m_pMaterialFunctor(materialFunctor)
    {
        Q_ASSERT_X(m_pShaderFunctor, Q_FUNC_INFO, "Shader functor should not be null!");
        Q_ASSERT_X(m_pTextureFunctor, Q_FUNC_INFO, "Texture functor should not be null!");
        Q_ASSERT_X(m_pMaterialFunctor, Q_FUNC_INFO, "Material functor should not be null!");
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

    void RenderModelPass::setIfRequired(const RenderModelBatchGroupKey &key, OpenGLShaderProgram *&shaderProgram, RenderMaterialPointer &material)
    {
        OpenGLShaderProgram* newShaderProgram = (*m_pShaderFunctor)(key.shaderId());
        changeShaderIfDifferent(shaderProgram, newShaderProgram);

        RenderMaterialPointer newMaterial = (*m_pMaterialFunctor)(key.materialId());
        changeMaterialIfDifferent(material, newMaterial);
    }

    void RenderModelPass::drawAllBatchGroups()
    {
        OpenGLShaderProgram* currentShaderProgram = nullptr;
        RenderMaterialPointer currentMaterial;

        foreach ( const RenderModelBatchGroupPointer &batchGroup, m_BatchGroups.values() )
        {
            setIfRequired(batchGroup->key(), currentShaderProgram, currentMaterial);
            batchGroup->drawAllBatches(currentShaderProgram);
        }

        changeShaderIfDifferent(currentShaderProgram, nullptr);
        changeMaterialIfDifferent(currentMaterial, RenderMaterialPointer());
    }

    void RenderModelPass::changeMaterialIfDifferent(Renderer::RenderMaterialPointer &origMaterial,
                                  const Renderer::RenderMaterialPointer &newMaterial)
    {
        if ( origMaterial == newMaterial )
            return;

        if ( newMaterial.isNull() )
        {
            QMap<ShaderDefs::TextureUnit, quint32> defaultMap;
            defaultMap.insert(ShaderDefs::MainTexture, 0);
            setTextureUnitMap(defaultMap);
        }
        else
        {
            setTextureUnitMap(newMaterial->textureUnitMap());
        }

        origMaterial = newMaterial;
    }

    void RenderModelPass::setTextureUnitMap(const QMap<ShaderDefs::TextureUnit, quint32> &map)
    {
        typedef QMap<ShaderDefs::TextureUnit, quint32> TextureUnitMap;

        foreach ( quint32 texture, m_TextureUnitMap.values() )
        {
            OpenGLTexturePointer tex = (*m_pTextureFunctor)(texture);
            if ( tex.isNull() )
                continue;

            tex->release();
        }

        m_TextureUnitMap.clear();
        m_TextureUnitMap = map;

        for ( TextureUnitMap::const_iterator it = m_TextureUnitMap.constBegin(); it != m_TextureUnitMap.constEnd(); ++it )
        {
            OpenGLTexturePointer tex = (*m_pTextureFunctor)(it.value());
            if ( tex.isNull() )
                continue;

            qDebug().nospace() << "Binding texture " << tex->textureStoreId() << " (" << it.value() << ") to point " << it.key();
            tex->bind(it.key());
        }
    }
}
