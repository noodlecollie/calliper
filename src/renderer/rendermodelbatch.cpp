#include "rendermodelbatch.h"
#include "openglerrors.h"
#include "openglhelpers.h"
#include "ishaderspec.h"
#include <QOpenGLShaderProgram>

namespace NS_RENDERER
{
    RenderModelBatch::RenderModelBatch(QOpenGLBuffer::UsagePattern usagePattern, IShaderSpec* shaderSpec,
                                       QOpenGLShaderProgram* shaderProgram, QObject* parent)
        : QObject(parent),
          m_iVAOID(0),
          m_iUsagePattern(usagePattern),
          m_bCreated(false),
          m_GlVertexBuffer(QOpenGLBuffer::VertexBuffer),
          m_GlIndexBuffer(QOpenGLBuffer::IndexBuffer),
          m_GlUniformBuffer(QOpenGLBuffer::StaticDraw),
          m_pShaderSpec(shaderSpec),
          m_pShaderProgram(shaderProgram),
          m_bDataStale(false),
          m_iBatchIdMask(batchIdMask(m_pShaderSpec->maxBatchedItems())),
          m_iUniformBlockIndex(0)
    {
    }

    RenderModelBatch::~RenderModelBatch()
    {
        destroy();
    }

    bool RenderModelBatch::create()
    {
        if ( m_bCreated )
            return true;

        GL_CURRENT_F;

        f->glGenVertexArrays(1, &m_iVAOID);
        f->glBindVertexArray(m_iVAOID);

        m_iUniformBlockIndex = f->glGetUniformBlockIndex(m_pShaderProgram->programId(), ShaderDefs::LOCAL_UNIFORM_BLOCK_NAME);
        f->glUniformBlockBinding(m_pShaderProgram->programId(), m_iUniformBlockIndex, 0);

        m_GlVertexBuffer.setUsagePattern(m_iUsagePattern);
        m_GlIndexBuffer.setUsagePattern(m_iUsagePattern);
        m_GlUniformBuffer.setUsagePattern(m_iUsagePattern);

        m_bCreated = m_GlVertexBuffer.create() && m_GlIndexBuffer.create() && m_GlUniformBuffer.create();
        return m_bCreated;
    }

    void RenderModelBatch::destroy()
    {
        if ( !m_bCreated )
            return;

        GL_CURRENT_F;

        m_GlVertexBuffer.destroy();
        m_GlIndexBuffer.destroy();
         m_GlUniformBuffer.destroy();

        f->glDeleteVertexArrays(1, &m_iVAOID);

        m_bCreated = false;
    }

    void RenderModelBatch::addItem(const RenderModelBatchParams &params)
    {
        Q_ASSERT_X(m_Items.count() < m_pShaderSpec->maxBatchedItems(),
                   Q_FUNC_INFO,
                   "Maximum number of batched items has already been reached");

        int newVertexOffset = 0;
        if ( !m_Items.isEmpty() )
        {
            newVertexOffset = m_Items.last().vertexOffset() + m_Items.last().vertexCount();
        }

        int oldVertexCount = m_LocalPositionBuffer.count() / m_pShaderSpec->positionComponents();
        int newVertexCount = params.vertexCount();

        resizeAllBuffers(oldVertexCount + newVertexCount);
        copyInData(params, oldVertexCount);
        addObjectIdsToPositions(newVertexOffset, newVertexCount, m_Items.count());

        int newIndexOffset = 0;
        if ( !m_Items.isEmpty() )
        {
            newIndexOffset = m_Items.last().indexOffset() + m_Items.last().indexCount();
        }

        int newIndexCount = params.indexCount();
        addIndices(params.indices(), newIndexCount, newIndexOffset);

        m_ModelToWorldMatrices.append(params.modelToWorldMatrix());

        m_Items.append(RenderModelBatchItem(newVertexOffset, newVertexCount, newIndexOffset, newIndexCount));
        m_bDataStale = true;
    }

    void RenderModelBatch::addIndices(const quint32 *source, int count, int indexOffset)
    {
        quint32 indexValueOffset = (m_LocalPositionBuffer.count()
                                    / m_pShaderSpec->positionComponents())
                                    - count;
        m_LocalIndexBuffer.resize(m_LocalIndexBuffer.count() + count);
        quint32* destination = m_LocalIndexBuffer.data() + indexOffset;

        for ( int i = 0; i < count; i++ )
        {
            destination[i] = source[i] + indexValueOffset;
        }
    }

    void RenderModelBatch::copyInData(const RenderModelBatchParams &params, int vertexCount)
    {
        // Construct some padding zeroes for unspecified data.
        // We need to set this to match the attribute  with the
        // largest number of components.
        QVector<float> padding;
        if ( params.someAttributesUnspecified() )
        {
            int components = maxComponentsFromVertexSpec();
            padding.insert(0, params.vertexCount() * components, 0);
        }

        memcpy(m_LocalPositionBuffer.data() + (vertexCount * m_pShaderSpec->positionComponents()),
               params.positions(),
               params.vertexCount() * m_pShaderSpec->positionComponents() * sizeof(float));

        memcpy(m_LocalNormalBuffer.data() + (vertexCount * m_pShaderSpec->normalComponents()),
               params.hasNormals() ? params.normals() : padding.constData(),
               params.vertexCount() * m_pShaderSpec->normalComponents() * sizeof(float));

        memcpy(m_LocalColorBuffer.data() + (vertexCount * m_pShaderSpec->colorComponents()),
               params.hasColors() ? params.colors() : padding.constData(),
               params.vertexCount() * m_pShaderSpec->colorComponents() * sizeof(float));

        memcpy(m_LocalTextureCoordinateBuffer.data() + (vertexCount * m_pShaderSpec->textureCoordinateComponents()),
               params.hasTextureCoordinates() ? params.textureCoordinates() : padding.constData(),
               params.vertexCount() * m_pShaderSpec->textureCoordinateComponents() * sizeof(float));
    }

    void RenderModelBatch::resizeAllBuffers(int numVertices)
    {
        Q_ASSERT_X(m_pShaderSpec, Q_FUNC_INFO, "Shader spec required!");

        m_LocalPositionBuffer.resize(numVertices * m_pShaderSpec->positionComponents());
        m_LocalNormalBuffer.resize(numVertices * m_pShaderSpec->normalComponents());
        m_LocalColorBuffer.resize(numVertices * m_pShaderSpec->colorComponents());
        m_LocalTextureCoordinateBuffer.resize(numVertices * m_pShaderSpec->textureCoordinateComponents());
    }

    int RenderModelBatch::maxComponentsFromVertexSpec() const
    {
        int components = 0;

        if ( m_pShaderSpec->positionComponents() > components )
            components = m_pShaderSpec->positionComponents();

        if ( m_pShaderSpec->normalComponents() > components )
            components = m_pShaderSpec->normalComponents();

        if ( m_pShaderSpec->colorComponents() > components )
            components = m_pShaderSpec->colorComponents();

        if ( m_pShaderSpec->textureCoordinateComponents() > components )
            components = m_pShaderSpec->textureCoordinateComponents();

        return components;
    }

    int RenderModelBatch::itemCount() const
    {
        return m_Items.count();
    }

    const IShaderSpec* RenderModelBatch::shaderSpec() const
    {
        return m_pShaderSpec;
    }

    void RenderModelBatch::upload(bool force)
    {
        if ( force || m_bDataStale )
        {
            uploadVertexData();

            m_GlIndexBuffer.bind();
            m_GlIndexBuffer.allocate(m_LocalIndexBuffer.constData(), m_LocalIndexBuffer.count() * sizeof(quint32));
            m_GlIndexBuffer.release();

            uploadUniformData();

            m_bDataStale = false;
        }
    }

    bool RenderModelBatch::needsUpload() const
    {
        return m_bDataStale;
    }

    void RenderModelBatch::uploadVertexData()
    {
        int size = (m_LocalPositionBuffer.count()
                    + m_LocalNormalBuffer.count()
                    + m_LocalColorBuffer.count()
                    + m_LocalTextureCoordinateBuffer.count())
                * sizeof(float);

        m_GlVertexBuffer.bind();
        m_GlVertexBuffer.allocate(size);

        int offset = 0;
        writeToGlVertexBuffer(m_LocalPositionBuffer, offset);
        writeToGlVertexBuffer(m_LocalNormalBuffer, offset);
        writeToGlVertexBuffer(m_LocalColorBuffer, offset);
        writeToGlVertexBuffer(m_LocalTextureCoordinateBuffer, offset);

        m_GlVertexBuffer.release();
    }

    void RenderModelBatch::writeToGlVertexBuffer(const QVector<float> &buffer, int &offset)
    {
        m_GlVertexBuffer.write(offset, buffer.constData(), buffer.count() * sizeof(float));
        offset += buffer.count() * sizeof(float);
    }

    int RenderModelBatch::localPositionCount() const
    {
        return m_LocalPositionBuffer.count();
    }

    int RenderModelBatch::localNormalCount() const
    {
        return m_LocalNormalBuffer.count();
    }

    int RenderModelBatch::localColorCount() const
    {
        return m_LocalColorBuffer.count();
    }

    int RenderModelBatch::localTextureCoordinateCount() const
    {
        return m_LocalTextureCoordinateBuffer.count();
    }

    int RenderModelBatch::localIndexCount() const
    {
        return m_LocalIndexBuffer.count();
    }

    void RenderModelBatch::clearItems()
    {
        m_LocalPositionBuffer.clear();
        m_LocalNormalBuffer.clear();
        m_LocalColorBuffer.clear();
        m_LocalTextureCoordinateBuffer.clear();
        m_LocalIndexBuffer.clear();
        m_ModelToWorldMatrices.clear();
        m_Items.clear();

        m_bDataStale = true;
    }

    QOpenGLShaderProgram* RenderModelBatch::shaderProgram() const
    {
        return m_pShaderProgram;
    }

    void RenderModelBatch::beginDraw()
    {
        bindVAO();

        // TODO: Really the attribute arrays don't belong in here,
        // and neither does the VAO. The arrays we use to draw with
        // are specific to a shader, so we only need to change them
        // at the beginning and end of a phase. It doesn't really make
        // sense to enable and disable them after every batch.
        // Once the actual render manager is in place, move these there.
        m_pShaderProgram->enableAttributeArray(ShaderDefs::PositionAttribute);
        m_pShaderProgram->enableAttributeArray(ShaderDefs::ColorAttribute);
    }

    void RenderModelBatch::endDraw()
    {
        m_GlIndexBuffer.release();
        m_GlVertexBuffer.release();
        m_pShaderProgram->disableAttributeArray(ShaderDefs::PositionAttribute);
        m_pShaderProgram->disableAttributeArray(ShaderDefs::ColorAttribute);
        releaseVAO();
    }

    void RenderModelBatch::setAttributePointers()
    {
        Q_ASSERT_X(!m_bDataStale, Q_FUNC_INFO, "Data not uploaded before setting attribute pointers!");
        Q_ASSERT_X(m_pShaderProgram, Q_FUNC_INFO, "Setting attribute pointers requires a shader program!");
        Q_ASSERT_X(m_pShaderSpec, Q_FUNC_INFO, "Setting attribute pointers requires a shader spec!");

        m_GlVertexBuffer.bind();

        int offset = 0;

        trySetAttributeBuffer(offset,
                              ShaderDefs::PositionAttribute,
                              m_pShaderSpec->positionComponents(),
                              m_LocalPositionBuffer.count());

        trySetAttributeBuffer(offset,
                              ShaderDefs::NormalAttribute,
                              m_pShaderSpec->normalComponents(),
                              m_LocalNormalBuffer.count());

        trySetAttributeBuffer(offset,
                              ShaderDefs::ColorAttribute,
                              m_pShaderSpec->colorComponents(),
                              m_LocalColorBuffer.count());

        trySetAttributeBuffer(offset,
                              ShaderDefs::TextureCoordinateAttribute,
                              m_pShaderSpec->textureCoordinateComponents(),
                              m_LocalTextureCoordinateBuffer.count());
    }

    void RenderModelBatch::trySetAttributeBuffer(int &offset, ShaderDefs::VertexArrayAttribute attribute, int components, int count)
    {
        if ( components > 0 )
        {
            m_pShaderProgram->setAttributeBuffer(attribute,
                                                 GL_FLOAT,
                                                 offset * sizeof(GLfloat),
                                                 components);
            offset += count;
        }
    }

    void RenderModelBatch::bindVAO()
    {
        GL_CURRENT_F;

        f->glBindVertexArray(m_iVAOID);
    }

    void RenderModelBatch::releaseVAO()
    {
        GL_CURRENT_F;

        f->glBindVertexArray(0);
    }

    void RenderModelBatch::draw()
    {
        GL_CURRENT_F;

        m_GlIndexBuffer.bind();
        f->glDrawElements(GL_TRIANGLES, m_LocalIndexBuffer.count(), GL_UNSIGNED_INT, (void*)0);
    }

    void RenderModelBatch::uploadUniformData()
    {
        // Not sure if this exact set of steps is required, but it's the
        // only way I got it to actually work.
        m_GlUniformBuffer.bind();
        m_GlUniformBuffer.allocate(m_Items.count() * 16 * sizeof(float));
        m_GlUniformBuffer.release();

        m_GlUniformBuffer.bindToIndex(0);

        m_GlUniformBuffer.bind();
        for ( int i = 0; i < m_Items.count(); i++ )
        {
            m_GlUniformBuffer.write(i * 16 * sizeof(float), m_ModelToWorldMatrices.at(i).constData(), 16 * sizeof(float));
        }
        m_GlUniformBuffer.release();
    }

    bool RenderModelBatch::shaderSupportsBatching() const
    {
        return m_pShaderSpec->maxBatchedItems() > 1;
    }

    void RenderModelBatch::addObjectIdsToPositions(int vertexOffset, int vertexCount, quint32 id)
    {
        Q_ASSERT_X(sizeof(float) == sizeof(quint32), Q_FUNC_INFO, "Size of float and quint32 do not match!");

        if ( !shaderSupportsBatching() )
            return;

        int numComponents = m_pShaderSpec->positionComponents();
        float* data = m_LocalPositionBuffer.data() + (vertexOffset * numComponents);

        for ( int i = 0; i < vertexCount; i++ )
        {
            // Get to the last component.
            data += numComponents - 1;

            // Set the bits.
            quint32 temp = (quint32)(*data);
            temp |= id & m_iBatchIdMask;
            *data = (float)temp;

            // Advance past the last component.
            data++;
        }
    }
}
