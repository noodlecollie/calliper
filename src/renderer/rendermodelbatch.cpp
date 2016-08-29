#include "rendermodelbatch.h"
#include "openglerrors.h"
#include "openglhelpers.h"
#include "shaderdefs.h"

namespace NS_RENDERER
{
    const int RenderModelBatch::MAX_ITEMS = 8;

    RenderModelBatch::RenderModelBatch(QOpenGLBuffer::UsagePattern usagePattern, QObject *parent)
        : QObject(parent),
          m_iUsagePattern(usagePattern),
          m_iVAOID(0)
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

        int newBaseOffsetFloats = 0;
        if ( !m_Items.isEmpty() )
        {
            newBaseOffsetFloats = m_Items.last().offsetFloats() + m_Items.last().countFloats();
        }

        int newCountFloats = params.vertexCount() * ShaderDefs::VertexAttributeLocationCount;

        m_VertexBuffer.resize(m_VertexBuffer.count() + newCountFloats);

        QVector<float> padding;
        if ( params.someAttributesUnspecified() )
        {
            padding.resize(params.vertexCount());
            memset(padding.data(), 0, params.vertexCount() * sizeof(float));
        }

        float* vertexBufferData = m_VertexBuffer.data() + newBaseOffsetFloats;

        copyInVertexData(vertexBufferData, params.positions(), params.vertexCount());
        copyInVertexData(vertexBufferData, params.hasNormals() ? params.normals() : padding.data(), params.vertexCount());
        copyInVertexData(vertexBufferData, params.hasColors() ? params.colors() : padding.data(), params.vertexCount());
        copyInVertexData(vertexBufferData, params.hasTextureCoordinates() ? params.textureCoordinates() : padding.data(), params.vertexCount());

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
}
