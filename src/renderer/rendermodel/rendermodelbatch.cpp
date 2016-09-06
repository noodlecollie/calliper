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
            dest[i] += oldCount;
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
          m_iUniformBlockIndex(0),
          m_bUsedObjectIds(new bool[m_iBatchSize])
    {
        memset(m_bUsedObjectIds.data(), 0, m_iBatchSize * sizeof(bool));
        clearUploadMetadata();
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

    bool RenderModelBatch::add(const RenderModelBatchParams &params)
    {
        const QMatrix4x4 &mat = params.modelToWorldMatrix();
        RenderModelBatchItem* item = NULL;

        if ( !m_ItemTable.contains(mat) )
        {
            if ( m_ItemTable.count() >= m_iBatchSize )
                return false;

            item = new RenderModelBatchItem();
            item->m_iObjectId = getNextObjectId();
            m_ItemTable.insert(mat, item);
        }

        foreach ( const GeometrySection &section, params.sections() )
        {
            item->m_Positions.append(section.vertexConstVector(GeometrySection::PositionAttribute));
            item->m_Positions.append(section.vertexConstVector(GeometrySection::NormalAttribute));
            item->m_Positions.append(section.vertexConstVector(GeometrySection::ColorAttribute));
            item->m_Positions.append(section.vertexConstVector(GeometrySection::TextureCoordinateAttribute));
            copyIndices(item->m_Indices, section.indexConstVector());
        }

        addObjectIdsToPositions(item);

        m_bDataStale = true;
        return true;
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
            uploadVertexData();
            uploadIndexData();
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

    void RenderModelBatch::uploadVertexData()
    {
        m_GlVertexBuffer.bind();
        m_GlVertexBuffer.allocate(getVertexDataCountInBytes());

        int offsetBytes = 0;
        clearUploadMetadata();

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
                        addObjectIdsToPositions(item);
                        uploadVertexData(item->m_Positions, offsetBytes);
                        m_UploadMetadata.numPositions += item->m_Positions.count();
                        break;
                    }

                    case 1:
                    {
                        uploadVertexData(item->m_Normals, offsetBytes);
                        m_UploadMetadata.numNormals += item->m_Normals.count();
                        break;
                    }

                    case 2:
                    {
                        uploadVertexData(item->m_Colors, offsetBytes);
                        m_UploadMetadata.numColors += item->m_Colors.count();
                        break;
                    }

                    case 3:
                    {
                        uploadVertexData(item->m_TextureCoordinates, offsetBytes);
                        m_UploadMetadata.numTexCoords += item->m_TextureCoordinates.count();
                        break;
                    }

                    default:
                        break;
                }
            }
        }

        m_GlVertexBuffer.release();
    }

    void RenderModelBatch::uploadIndexData()
    {
        m_GlIndexBuffer.bind();
        m_GlIndexBuffer.allocate(getIndexDataCountInBytes());

        quint32* data = reinterpret_cast<quint32*>(m_GlIndexBuffer.map(QOpenGLBuffer::WriteOnly));

        int delta = 0;
        foreach ( RenderModelBatchItem* item, m_ItemTable.values() )
        {
            for ( int i = 0; i < item->m_Indices.count(); i++ )
            {
                *data = item->m_Indices.at(i) + delta;
            }

            delta += item->m_Indices.count();
        }

        m_GlIndexBuffer.release();
    }

    void RenderModelBatch::uploadUniformData()
    {
        // Not sure if this exact set of steps is required, but it's the
        // only way I got it to actually work.
        m_GlUniformBuffer.bind();
        m_GlUniformBuffer.allocate(m_ItemTable.count() * 16 * sizeof(float));
        m_GlUniformBuffer.release();

        m_GlUniformBuffer.bindToIndex(ShaderDefs::LocalUniformBlockBindingPoint);

        m_GlUniformBuffer.bind();
        int i = 0;
        foreach ( const QMatrix4x4 &mat, m_ItemTable.keys() )
        {
            m_GlUniformBuffer.write(i * 16 * sizeof(float), mat.constData(), 16 * sizeof(float));
            i++;
        }
        m_GlUniformBuffer.release();
    }

    void RenderModelBatch::addObjectIdsToPositions(RenderModelBatchItem* item)
    {
        Q_ASSERT_X(sizeof(float) == sizeof(quint32), Q_FUNC_INFO, "Size of float and quint32 do not match!");

        int numComponents = m_VertexFormat.positionComponents();
        float* data = item->m_Positions.data();
        int vertexCount = item->m_Positions.count() / numComponents;

        for ( int i = 0; i < vertexCount; i++ )
        {
            // Get to the last component.
            data += numComponents - 1;

            // Set the bits.
            quint32 temp = (quint32)(*data);
            temp |= item->m_iObjectId & m_iBatchIdMask;
            *data = (float)temp;

            // Advance past the last component.
            data++;
        }
    }

    // Assumes we've already checked that there is an ID available.
    quint32 RenderModelBatch::getNextObjectId()
    {
        for ( int i = 0; i < m_iBatchSize; i++ )
        {
            if ( !m_bUsedObjectIds[i] )
            {
                m_bUsedObjectIds[i] = true;
                return i;
            }
        }

        return m_iBatchSize;
    }

    void RenderModelBatch::invalidateObjectId(quint32 id)
    {
        m_bUsedObjectIds[id] = false;
    }

    void RenderModelBatch::trySetAttributeBuffer(QOpenGLShaderProgram *shaderProgram, int &offset,
                                                 ShaderDefs::VertexArrayAttribute attribute,
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

    void RenderModelBatch::clearUploadMetadata()
    {
        memset(&m_UploadMetadata, 0, sizeof(UploadMetadata));
    }

    bool RenderModelBatch::canAddNewItem(const QMatrix4x4 &key) const
    {
        return m_ItemTable.count() < m_iBatchSize
                || containsKey(key);
    }

    bool RenderModelBatch::containsKey(const QMatrix4x4 &mat) const
    {
        return m_ItemTable.contains(mat);
    }
}
