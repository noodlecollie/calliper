#include "rendermodelbatch.h"
#include "openglerrors.h"
#include "openglhelpers.h"
#include "shaderdefs.h"
#include "ishaderspec.h"

namespace NS_RENDERER
{
    const int RenderModelBatch::MAX_ITEMS = 8;

    RenderModelBatch::RenderModelBatch(QOpenGLBuffer::UsagePattern usagePattern, QObject *parent)
        : QObject(parent),
          m_iVAOID(0),
          m_iUsagePattern(usagePattern),
          m_bCreated(false),
          m_GlVertexBuffer(QOpenGLBuffer::VertexBuffer),
          m_GlIndexBuffer(QOpenGLBuffer::IndexBuffer),
          m_pShaderSpec(NULL),
          m_bDataStale(false)
    {
    }

    RenderModelBatch::~RenderModelBatch()
    {
        destroy();
    }

    GLuint RenderModelBatch::vaoHandle() const
    {
        return m_iVAOID;
    }

    bool RenderModelBatch::create()
    {
        if ( m_bCreated )
            return true;

        GL_CURRENT_F;

        GLTRY(f->glGenVertexArrays(1, &m_iVAOID));
        GLTRY(f->glBindVertexArray(m_iVAOID));

        m_GlVertexBuffer.setUsagePattern(m_iUsagePattern);
        m_GlIndexBuffer.setUsagePattern(m_iUsagePattern);

        m_bCreated = m_GlVertexBuffer.create() && m_GlIndexBuffer.create();
        return m_bCreated;
    }

    void RenderModelBatch::destroy()
    {
        if ( !m_bCreated )
            return;

        GL_CURRENT_F;

        m_GlVertexBuffer.destroy();
        m_GlIndexBuffer.destroy();

        GLTRY(f->glDeleteVertexArrays(1, &m_iVAOID));

        m_bCreated = false;
    }

    void RenderModelBatch::addItem(const RenderModelBatchParams &params)
    {
        if ( m_Items.count() >= MAX_ITEMS )
            return;

        if ( !m_pShaderSpec )
            return;

        int newBaseOffsetFloats = 0;
        if ( !m_Items.isEmpty() )
        {
            newBaseOffsetFloats = m_Items.last().offsetFloats() + m_Items.last().countFloats();
        }

        // Calculate how many floats we need to store the vertex data.
        // This is dependent on how many vertices we have, and how many
        // components there are per vertex.
        int newCountFloats = m_pShaderSpec->totalVertexComponents() * params.vertexCount();

        m_LocalVertexBuffer.resize(m_LocalVertexBuffer.count() + newCountFloats);

        QVector<float> padding;
        if ( params.someAttributesUnspecified() )
        {
            int components = maxComponentsFromVertexSpec();
            padding.insert(0, params.vertexCount() * components, 0);
        }

        float* vertexBufferData = m_LocalVertexBuffer.data() + newBaseOffsetFloats;
        const float* paddingData = padding.constData();

        copyInVertexData(vertexBufferData,
                         params.positions(),
                         params.vertexCount() * m_pShaderSpec->positionComponents());

        copyInVertexData(vertexBufferData,
                         params.hasNormals() ? params.normals() : paddingData,
                         params.vertexCount() * m_pShaderSpec->normalComponents());

        copyInVertexData(vertexBufferData,
                         params.hasColors() ? params.colors() : paddingData,
                         params.vertexCount() * m_pShaderSpec->colorComponents());

        copyInVertexData(vertexBufferData,
                         params.hasTextureCoordinates() ? params.textureCoordinates() : paddingData,
                         params.vertexCount() * m_pShaderSpec->textureCoordinateComponents());

        int newBaseIndexOffsetInts = 0;
        if ( !m_Items.isEmpty() )
        {
            newBaseIndexOffsetInts = m_Items.last().indexOffsetInts() + m_Items.last().indexCountInts();
        }

        int newIndexCountInts = params.indexCount();
        m_LocalIndexBuffer.resize(m_LocalIndexBuffer.count() + newIndexCountInts);
        quint32* indexData = m_LocalIndexBuffer.data();
        copyInIndexData(indexData, params.indices(), newIndexCountInts);

        m_ModelToWorldMatrices.append(params.modelToWorldMatrix());

        m_Items.append(RenderModelBatchItem(newBaseOffsetFloats, newCountFloats, newBaseIndexOffsetInts, newIndexCountInts));
        m_bDataStale = true;
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

    void RenderModelBatch::copyInVertexData(float* &dest, const float *source, int floatCount)
    {
        memcpy(dest, source, floatCount * sizeof(float));
        dest += floatCount;
    }

    void RenderModelBatch::copyInIndexData(quint32 *&dest, const quint32 *source, int intCount)
    {
        memcpy(dest, source, intCount * sizeof(quint32));
        dest += intCount;
    }

    int RenderModelBatch::itemCount() const
    {
        return m_Items.count();
    }

    const IShaderSpec* RenderModelBatch::shaderSpec() const
    {
        return m_pShaderSpec;
    }

    void RenderModelBatch::setShaderSpec(const IShaderSpec *spec)
    {
        m_pShaderSpec = spec;
    }

    void RenderModelBatch::upload(bool force)
    {
        if ( force || m_bDataStale )
        {
            m_GlVertexBuffer.bind();
            m_GlVertexBuffer.allocate(m_LocalVertexBuffer.constData(), m_LocalVertexBuffer.count() * sizeof(float));

            m_GlIndexBuffer.bind();
            m_GlIndexBuffer.allocate(m_LocalIndexBuffer.constData(), m_LocalIndexBuffer.count() * sizeof(quint32));

            m_bDataStale = false;
        }
    }

    bool RenderModelBatch::needsUpload() const
    {
        return m_bDataStale;
    }
}
