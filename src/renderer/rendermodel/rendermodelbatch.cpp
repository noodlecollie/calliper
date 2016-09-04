#include "rendermodelbatch.h"
#include "opengl/openglerrors.h"
#include "opengl/openglhelpers.h"
#include "shaders/ishaderspec.h"
#include <QOpenGLShaderProgram>

namespace NS_RENDERER
{
    RenderModelBatch::RenderModelBatch(QOpenGLBuffer::UsagePattern usagePattern, const VertexFormat &vertexFormat, int batchSize)
        : m_iUsagePattern(usagePattern),
          m_bCreated(false),
          m_GlVertexBuffer(QOpenGLBuffer::VertexBuffer),
          m_GlIndexBuffer(QOpenGLBuffer::IndexBuffer),
          m_GlUniformBuffer(QOpenGLBuffer::StaticDraw),
          m_VertexFormat(vertexFormat),
          m_iBatchSize(batchSize),
          m_bDataStale(false),
          m_iBatchIdMask(batchIdMask(m_iBatchSize)),
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

        m_GlVertexBuffer.destroy();
        m_GlIndexBuffer.destroy();
        m_GlUniformBuffer.destroy();

        m_bCreated = false;
    }

    void RenderModelBatch::addItem(const RenderModelBatchParams &params)
    {
        Q_ASSERT_X(m_Items.count() < m_iBatchSize,
                   Q_FUNC_INFO,
                   "Maximum number of batched items has already been reached");

        int newVertexOffset = 0;
        if ( !m_Items.isEmpty() )
        {
            newVertexOffset = m_Items.last().vertexOffset() + m_Items.last().vertexCount();
        }

        int oldVertexCount = m_LocalPositionBuffer.count() / m_VertexFormat.positionComponents();
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
                                    / m_VertexFormat.positionComponents())
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

        memcpy(m_LocalPositionBuffer.data() + (vertexCount * m_VertexFormat.positionComponents()),
               params.positions(),
               params.vertexCount() * m_VertexFormat.positionComponents() * sizeof(float));

        memcpy(m_LocalNormalBuffer.data() + (vertexCount * m_VertexFormat.normalComponents()),
               params.hasNormals() ? params.normals() : padding.constData(),
               params.vertexCount() * m_VertexFormat.normalComponents() * sizeof(float));

        memcpy(m_LocalColorBuffer.data() + (vertexCount * m_VertexFormat.colorComponents()),
               params.hasColors() ? params.colors() : padding.constData(),
               params.vertexCount() * m_VertexFormat.colorComponents() * sizeof(float));

        memcpy(m_LocalTextureCoordinateBuffer.data() + (vertexCount * m_VertexFormat.textureCoordinateComponents()),
               params.hasTextureCoordinates() ? params.textureCoordinates() : padding.constData(),
               params.vertexCount() * m_VertexFormat.textureCoordinateComponents() * sizeof(float));
    }

    void RenderModelBatch::resizeAllBuffers(int numVertices)
    {
        m_LocalPositionBuffer.resize(numVertices * m_VertexFormat.positionComponents());
        m_LocalNormalBuffer.resize(numVertices * m_VertexFormat.normalComponents());
        m_LocalColorBuffer.resize(numVertices * m_VertexFormat.colorComponents());
        m_LocalTextureCoordinateBuffer.resize(numVertices * m_VertexFormat.textureCoordinateComponents());
    }

    int RenderModelBatch::maxComponentsFromVertexSpec() const
    {
        int components = 0;

        if ( m_VertexFormat.positionComponents() > components )
            components = m_VertexFormat.positionComponents();

        if ( m_VertexFormat.normalComponents() > components )
            components = m_VertexFormat.normalComponents();

        if ( m_VertexFormat.colorComponents() > components )
            components = m_VertexFormat.colorComponents();

        if ( m_VertexFormat.textureCoordinateComponents() > components )
            components = m_VertexFormat.textureCoordinateComponents();

        return components;
    }

    int RenderModelBatch::itemCount() const
    {
        return m_Items.count();
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

    void RenderModelBatch::setAttributePointers(QOpenGLShaderProgram *shaderProgram)
    {
        // TODO: Need to eliminate holding a pointer to the shader.
        Q_ASSERT_X(!m_bDataStale, Q_FUNC_INFO, "Data not uploaded before setting attribute pointers!");

        int offset = 0;

        trySetAttributeBuffer(shaderProgram,
                              offset,
                              ShaderDefs::PositionAttribute,
                              m_VertexFormat.positionComponents(),
                              m_LocalPositionBuffer.count());

        trySetAttributeBuffer(shaderProgram,
                              offset,
                              ShaderDefs::NormalAttribute,
                              m_VertexFormat.normalComponents(),
                              m_LocalNormalBuffer.count());

        trySetAttributeBuffer(shaderProgram,
                              offset,
                              ShaderDefs::ColorAttribute,
                              m_VertexFormat.colorComponents(),
                              m_LocalColorBuffer.count());

        trySetAttributeBuffer(shaderProgram,
                              offset,
                              ShaderDefs::TextureCoordinateAttribute,
                              m_VertexFormat.textureCoordinateComponents(),
                              m_LocalTextureCoordinateBuffer.count());
    }

    void RenderModelBatch::trySetAttributeBuffer(QOpenGLShaderProgram *shaderProgram, int &offset, ShaderDefs::VertexArrayAttribute attribute,
                                                 int components, int count)
    {
        if ( components > 0 )
        {
            shaderProgram->setAttributeBuffer(attribute,
                                                 GL_FLOAT,
                                                 offset * sizeof(GLfloat),
                                                 components);
            offset += count;
        }
    }

    void RenderModelBatch::bindDraw()
    {
        m_GlVertexBuffer.bind();
        m_GlIndexBuffer.bind();
    }

    void RenderModelBatch::draw()
    {
        GL_CURRENT_F;

        f->glDrawElements(GL_TRIANGLES, m_LocalIndexBuffer.count(), GL_UNSIGNED_INT, (void*)0);
    }

    void RenderModelBatch::releaseDraw()
    {
        m_GlIndexBuffer.release();
        m_GlVertexBuffer.release();
    }

    void RenderModelBatch::uploadUniformData()
    {
        // Not sure if this exact set of steps is required, but it's the
        // only way I got it to actually work.
        m_GlUniformBuffer.bind();
        m_GlUniformBuffer.allocate(m_Items.count() * 16 * sizeof(float));
        m_GlUniformBuffer.release();

        m_GlUniformBuffer.bindToIndex(ShaderDefs::LocalUniformBlockBindingPoint);

        m_GlUniformBuffer.bind();
        for ( int i = 0; i < m_Items.count(); i++ )
        {
            m_GlUniformBuffer.write(i * 16 * sizeof(float), m_ModelToWorldMatrices.at(i).constData(), 16 * sizeof(float));
        }
        m_GlUniformBuffer.release();
    }

    bool RenderModelBatch::supportsBatching() const
    {
        return m_iBatchSize > 1;
    }

    void RenderModelBatch::addObjectIdsToPositions(int vertexOffset, int vertexCount, quint32 id)
    {
        Q_ASSERT_X(sizeof(float) == sizeof(quint32), Q_FUNC_INFO, "Size of float and quint32 do not match!");

        if ( !supportsBatching() )
            return;

        int numComponents = m_VertexFormat.positionComponents();
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

    bool RenderModelBatch::isFull() const
    {
        return m_Items.count() >= m_iBatchSize;
    }
}
