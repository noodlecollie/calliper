#include "rendermodelbatch.h"
#include "opengl/openglerrors.h"
#include "opengl/openglhelpers.h"
#include "shaders/ishaderspec.h"
#include <QOpenGLShaderProgram>

namespace
{
    void copyIndices(QVector<quint32> &dest, const QVector<quint32> &source)
    {
        int oldCount = dest.count();
        dest.append(source);
        for ( int i = oldCount; i < dest.count(); i++ )
        {
            dest[i] += oldcount;
        }
    }
}

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
          m_iBatchIdMask(batchIdMask(bitsRequired(m_iBatchSize))),
          m_iUniformBlockIndex(0)
    {
    }

    RenderModelBatch::~RenderModelBatch()
    {
        clear();
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

    bool RenderModelBatch::supportsBatching() const
    {
        return m_iBatchSize > 1;
    }

    void RenderModelBatch::add(const RenderModelBatchParams &params)
    {
        const QMatrix4x4 &mat = params.modelToWorldMatrix();
        RenderModelBatchItem* item = NULL;

        if ( !m_ItemTable.contains(mat) )
        {
            if ( m_ItemTable.count() >= m_iBatchSize )
                return;

            item = new RenderModelBatchItem();
            m_ItemTable.insert(mat, item);
        }

        foreach ( const GeometrySection &section, params.sections() )
        {
            item->m_Positions.append(section.vertexConstVector(GeometrySection::PositionAttribute));
            item->m_Positions.append(section.vertexConstVector(GeometrySection::NormalAttribute));
            item->m_Positions.append(section.vertexConstVector(GeometrySection::ColorAttribute));
            item->m_Positions.append(section.vertexConstVector(GeometrySection::TextureCoordinateAttribute));
            copyData(item->m_Indices, section.indexConstVector);
        }

        m_bDataStale = true;
    }

    void RenderModelBatch::clear()
    {
        qDeleteAll(m_ItemTable.values());
        m_ItemTable.clear();
        m_bDataStale = true;
    }

    void RenderModelBatch::upload(bool force)
    {
        if ( force || m_bDataStale )
        {
            uploadAllVertexData();

            m_GlIndexBuffer.bind();
            m_GlIndexBuffer.allocate(m_LocalIndexBuffer.constData(), m_LocalIndexBuffer.count() * sizeof(quint32));
            m_GlIndexBuffer.release();

            uploadUniformData();

            m_bDataStale = false;
        }
    }

    int RenderModelBatch::getVertexDataCountInBytes() const
    {
        int count = 0;

        foreach ( RenderModelBatchItem* item, m_ItemTable.values() )
        {
            count += item->m_Positions.count() +
                    item->m_Normals.count() +
                    item->m_Colors.count() +
                    item->m_TextureCoordinates.count();
        }

        return count * sizeof(float);
    }

    int RenderModelBatch::getIndexDataCountInBytes() const
    {
        int count = 0;

        foreach ( RenderModelBatchItem* item, m_ItemTable.values() )
        {
            count += item->m_Indices.count();
        }

        return count * sizeof(quint32);
    }

    void RenderModelBatch::uploadVertexData(const QVector<float> source, int &offsetBytes)
    {
        m_GlVertexBuffer.write(offsetBytes, source.constData(), source.count() * sizeof(float));
        offsetBytes += source.count() * sizeof(float);
    }

    void RenderModelBatch::uploadAllVertexData()
    {
        m_GlVertexBuffer.bind();
        m_GlVertexBuffer.allocate(getVertexDataCountInBytes());

        int offsetBytes = 0;

        // We're gonna have to do this in passes, because we need to get all of the
        // position data and then all of the normal data, and then...
        for ( int i = 0; i < 4; i++ )
        {
            foreach ( RenderModelBatchItem* item, m_ItemTable.values() )
            {
                switch(i)
                {
                    case 0:
                    {
                        uploadVertexData(item->m_Positions, offsetBytes);
                        break;
                    }

                    case 1:
                    {
                        uploadVertexData(item->m_Normals, offsetBytes);
                        break;
                    }

                    case 2:
                    {
                        uploadVertexData(item->m_Colors, offsetBytes);
                        break;
                    }

                    case 3:
                    {
                        uploadVertexData(item->m_TextureCoordinates, offsetBytes);
                        break;
                    }

                    default:
                        break;
                }
            }
        }

        m_GlVertexBuffer.release();
    }

    void RenderModelBatch::uploadAllIndexData()
    {
        m_GlIndexBuffer.bind();
        m_GlIndexBuffer.allocate(getIndexDataCountInBytes());

        quint32* data = m_GlIndexBuffer.map(QOpenGLBuffer::WriteOnly);

        // TODO: Finish

        m_GlIndexBuffer.release();
    }
}
