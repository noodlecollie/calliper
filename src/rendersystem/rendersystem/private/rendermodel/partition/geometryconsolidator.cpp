#include "geometryconsolidator.h"

#include "renderutils.h"

#include "rendersystem/private/static-stores/openglshaderstore/openglshaderstore.h"

namespace
{
    inline quint32 maskFromNumberOfBits(int numBits)
    {
        return ~(static_cast<quint32>(0)) >> ((sizeof(quint32) * 8) - numBits);
    }

    inline quint8 bitsRequired(int maxValue)
    {
        for ( quint32 i = 0; i < sizeof(quint32) * 8; i++ )
        {
            if ( (1 << i) >= maxValue )
            {
                return i+1;
            }
        }

        return sizeof(quint32) * 8;
    }

    inline quint32 totalRequiredVertexFloats(int numPositions, const VertexFormat& format)
    {
        return format.totalVertexComponents() * numPositions;
    }
}

GeometryConsolidator::GeometryConsolidator(const RenderModelContext &context,
                                           RenderSystem::MaterialDefs::MaterialId materialId,
                                           GeometryDataContainer &data,
                                           GeometryOffsetTable &offsetTable)
    : m_Context(context),
      m_nMaterialId(materialId),
      m_GeometryDataContainer(data),
      m_OffsetTable(offsetTable),
      m_pShader(Q_NULLPTR),
      m_VertexData(),
      m_IndexData(),
      m_nItemsPerBatch(0),
      m_nCurrentObjectId(0),
      m_nObjectIdMask(0)
{

}

void GeometryConsolidator::setUpShader()
{
    const RenderSystem::ShaderDefs::ShaderId shaderId = RenderUtils::shaderFromMaterial(m_Context.renderMode(), m_nMaterialId);
    if ( shaderId != RenderSystem::ShaderDefs::UnknownShaderId )
    {
        m_pShader = OpenGLShaderStore::globalInstance()->object(shaderId);
    }
    else
    {
        m_pShader = Q_NULLPTR;
    }
}

void GeometryConsolidator::clear()
{
    m_VertexData.clear();
    m_IndexData.clear();
    m_OffsetTable.clear();
    m_nItemsPerBatch = m_pShader ? m_pShader->maxBatchedItems() : 0;
    m_nCurrentObjectId = 0;
    m_VertexFormat = m_pShader ? m_pShader->vertexFormat() : VertexFormat();

    if ( m_nItemsPerBatch > 1 )
    {
        m_nObjectIdMask = maskFromNumberOfBits(bitsRequired(m_nItemsPerBatch));
    }
    else
    {
        m_nObjectIdMask = 0;
    }
}

const QVector<float>& GeometryConsolidator::vertexData() const
{
    return m_VertexData;
}

const QVector<quint32>& GeometryConsolidator::indexData() const
{
    return m_IndexData;
}

void GeometryConsolidator::consolidate()
{
    setUpShader();

    if ( !m_pShader )
    {
        return;
    }

    clear();

    for ( GeometryDataContainer::ConstIterator it = m_GeometryDataContainer.constBegin();
          it != m_GeometryDataContainer.constEnd();
          ++it )
    {
        GeometryDataContainer::GeometryDataPointer geometry = it.value();
        consolidate(geometry);

        m_nCurrentObjectId = (m_nCurrentObjectId + 1) % m_nItemsPerBatch;
    }
}

void GeometryConsolidator::consolidate(const QSharedPointer<GeometryData> &geometry)
{
    m_OffsetTable.createNewItem();

    consolidateVertices(geometry);
    consolidateIndices(geometry);

    GeometryOffsetTable::ObjectOffsets& offsets = m_OffsetTable.lastItem();
    offsets.drawMode = geometry->drawMode();
    offsets.lineWidth = geometry->lineWidth();
}

void GeometryConsolidator::consolidateVertices(const QSharedPointer<GeometryData> &geometry)
{
    const VertexFormat vertexFormat = m_pShader->vertexFormat();
    GeometryOffsetTable::ObjectOffsets& offsets = m_OffsetTable.lastItem();
    offsets.vertexOffsetFloats = m_VertexData.count();
    offsets.vertexCountFloats = totalRequiredVertexFloats(geometry->positions().count(), vertexFormat);

    quint32 beginOffsetInFloats = offsets.vertexOffsetFloats;

    m_VertexData.resize(m_VertexData.count() + offsets.vertexCountFloats);

    consolidateVertices(geometry->positions(),
                        vertexFormat.positionComponents(),
                        beginOffsetInFloats,
                        m_nObjectIdMask > 0);

    beginOffsetInFloats += vertexFormat.positionComponents();

    consolidateVertices(geometry->normals(),
                        vertexFormat.normalComponents(),
                        beginOffsetInFloats,
                        false,
                        vertexFormat.normalComponents() > 0 ? geometry->positions().count() : 0,
                        QVector4D(0,0,0,1));

    beginOffsetInFloats += vertexFormat.normalComponents();

    consolidateVertices(geometry->colors(),
                        vertexFormat.colorComponents(),
                        beginOffsetInFloats,
                        false,
                        vertexFormat.colorComponents() > 0 ? geometry->positions().count() : 0,
                        QVector4D(1,1,1,1));

    beginOffsetInFloats += vertexFormat.colorComponents();

    consolidateVertices(geometry->textureCoordinates(),
                        vertexFormat.textureCoordinateComponents(),
                        beginOffsetInFloats,
                        false,
                        vertexFormat.textureCoordinateComponents() > 0 ? geometry->positions().count() : 0,
                        QVector4D(0,0,0,0));
}

void GeometryConsolidator::consolidateVertices(const QVector<QVector4D> &source,
                                               int components,
                                               quint32 offset,
                                               bool encodeId,
                                               int minimumItemCount,
                                               const QVector4D &substituteValue)
{
    int stride = m_pShader->vertexFormat().totalVertexComponents();

    for ( int srcItem = 0, destItem = offset;
          srcItem < source.count();
          ++srcItem, destItem += stride)
    {
        const QVector4D& vec = source[srcItem];

        for ( int curComponent = 0; curComponent < components; ++curComponent )
        {
            float value = vec[curComponent];

            if ( encodeId && curComponent == components - 1 )
            {
                quint32* valueAsInt = reinterpret_cast<quint32*>(&value);
                *valueAsInt |= (static_cast<quint32>(m_nCurrentObjectId) & m_nObjectIdMask);
            }

            Q_ASSERT_X(destItem + curComponent < m_VertexData.count(), Q_FUNC_INFO, "Index out of range!");
            m_VertexData[destItem + curComponent] = value;
        }
    }

    if ( minimumItemCount < 1 || source.count() >= minimumItemCount )
    {
        return;
    }

    const int extraItemsToAdd = minimumItemCount - source.count();
    const int firstItem = offset + (source.count() * stride);

    for ( int item = 0; item < extraItemsToAdd; ++item )
    {
        const int itemIndex = firstItem + (item * stride);

        for ( int curComponent = 0; curComponent < components; ++curComponent )
        {
            m_VertexData[itemIndex + curComponent] = substituteValue[curComponent];
        }
    }
}

void GeometryConsolidator::consolidateIndices(const QSharedPointer<GeometryData> &geometry)
{
    GeometryOffsetTable::ObjectOffsets& offsets = m_OffsetTable.lastItem();
    offsets.indexOffsetInts = m_IndexData.count();
    offsets.indexCountInts = geometry->computeTotalIndexBytes() / sizeof(quint32);

    quint32 indexIncrement = offsets.vertexOffsetFloats / m_VertexFormat.totalVertexComponents();

    m_IndexData.resize(m_IndexData.count() + offsets.indexCountInts);
    consolidateIndices(geometry->indices(), offsets.indexOffsetInts, indexIncrement);
}

void GeometryConsolidator::consolidateIndices(const QVector<quint32> &indices, quint32 offsetInInts, quint32 indexIncrement)
{
    for ( int i = 0; i < indices.count(); ++i )
    {
        m_IndexData[offsetInInts + i] = indices[i] + indexIncrement;
    }
}
