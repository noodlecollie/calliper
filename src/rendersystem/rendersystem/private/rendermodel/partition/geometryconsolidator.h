#ifndef GEOMETRYCONSOLIDATOR_H
#define GEOMETRYCONSOLIDATOR_H

#include <QVector>

#include "geometrydatacontainer.h"
#include "geometryoffsettable.h"

#include "rendersystem/private/rendermodel/rendermodelcontext.h"
#include "rendersystem/private/shaders/base/openglshaderprogram.h"
#include "rendersystem/interface-classes/definitions/materialdefs.h"

class GeometryConsolidator
{
public:

    GeometryConsolidator(const RenderModelContext& context,
                         RenderSystem::MaterialDefs::MaterialId materialId,
                         GeometryDataContainer& data,
                         GeometryOffsetTable& offsetTable);

    // Vertices have the appropriate object ID stored in their final position
    // component, if applicable.
    const QVector<float>& vertexData() const;
    const QVector<quint32>& indexData() const;

    void consolidate();
    void clear();

private:
    void setUpShader();
    void consolidate(const QSharedPointer<GeometryData>& geometry);

    void consolidateVertices(const QSharedPointer<GeometryData>& geometry);
    void consolidateVertices(const QVector<QVector4D> &source,
                             int components,
                             quint32 offset,
                             bool encodeId = false,
                             int minimumItemCount = 0,
                             const QVector4D& substituteValue = QVector4D());

    void consolidateIndices(const QSharedPointer<GeometryData>& geometry);
    void consolidateIndices(const QVector<quint32>& indices, quint32 offsetInInts, quint32 indexIncrement);

    const RenderModelContext& m_Context;
    const RenderSystem::MaterialDefs::MaterialId m_nMaterialId;

    GeometryDataContainer& m_GeometryDataContainer;
    GeometryOffsetTable& m_OffsetTable;
    OpenGLShaderProgram* m_pShader;

    QVector<float> m_VertexData;
    QVector<quint32> m_IndexData;

    int m_nItemsPerBatch;
    quint8 m_nCurrentObjectId;
    quint32 m_nObjectIdMask;
    VertexFormat m_VertexFormat;
};

#endif // GEOMETRYCONSOLIDATOR_H
