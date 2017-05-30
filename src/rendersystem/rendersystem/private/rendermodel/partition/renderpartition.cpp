#include "renderpartition.h"

#include "geometryrenderer.h"

namespace
{
    const QOpenGLBuffer::UsagePattern BUFFER_USAGE_PATTERN = QOpenGLBuffer::DynamicDraw;
}

RenderPartition::RenderPartition(const RenderModelContext &context,
                                 RenderSystem::PublicStoreDefs::MaterialId materialId,
                                 int maxItems)
    : m_Context(context),
      m_nMaterialId(materialId),
      m_nMaxItems(maxItems),
      m_GeometryDataContainer(),
      m_OpenGLBuffers(BUFFER_USAGE_PATTERN),
      m_OffsetTable(),
      m_Uploader(m_Context, m_nMaterialId, m_GeometryDataContainer, m_OffsetTable, m_OpenGLBuffers)
{

}

int RenderPartition::maxItems() const
{
    return m_nMaxItems;
}

int RenderPartition::itemCount() const
{
    return m_GeometryDataContainer.count();
}

bool RenderPartition::isFull() const
{
    return m_GeometryDataContainer.count() >= m_nMaxItems;
}

int RenderPartition::freeSpaces() const
{
    return m_nMaxItems - m_GeometryDataContainer.count();
}

void RenderPartition::setGeometry(const QSharedPointer<RenderSystem::GeometrySection> &section)
{
    if ( isFull() )
    {
        return;
    }

    m_GeometryDataContainer.insert(section);
}

void RenderPartition::removeGeometry(RenderSystem::PublicRenderModelDefs::ObjectId objectId)
{
    m_GeometryDataContainer.remove(objectId);
}

void RenderPartition::draw(GLenum drawMode, float lineWidth)
{
    if ( !m_Uploader.uploadIfRequired() )
    {
        return;
    }

    GeometryRenderer renderer(m_Context, m_nMaterialId, m_OffsetTable, m_OpenGLBuffers);

    renderer.setDrawMode(drawMode);
    renderer.setLineWidth(lineWidth);

    renderer.draw();
}