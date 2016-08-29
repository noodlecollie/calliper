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
          m_iUsagePattern(usagePattern),
          m_iVAOID(0),
          m_pShaderSpec(NULL),
          m_bCreated(false),
          m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
          m_IndexBuffer(QOpenGLBuffer::IndexBuffer)
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

        m_VertexBuffer.resize(m_VertexBuffer.count() + newCountFloats);

        QVector<float> padding;
        if ( params.someAttributesUnspecified() )
        {
            int components = maxComponentsFromVertexSpec();
            padding.insert(0, params.vertexCount() * components, 0);
        }

        float* vertexBufferData = m_VertexBuffer.data() + newBaseOffsetFloats;
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
        m_IndexBuffer.resize(m_IndexBuffer.count() + newIndexCountInts);
        quint32* indexData = m_IndexBuffer.data();
        copyInIndexData(indexData, params.indices(), newIndexCountInts);

        m_Items.append(RenderModelBatchItem(newBaseOffsetFloats, newCountFloats, newBaseIndexOffsetInts, newIndexCountInts));
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
        dest += floatCount * sizeof(float);
    }

    void RenderModelBatch::copyInIndexData(quint32 *&dest, const quint32 *source, int intCount)
    {
        memcpy(dest, source, intCount * sizeof(quint32));
        dest += intCount * sizeof(quint32);
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
}
