#ifndef GEOMETRYUPLOADER_H
#define GEOMETRYUPLOADER_H

#include <QSharedPointer>
#include <QException>

#include "openglbuffercollection.h"
#include "geometrydatacontainer.h"
#include "geometryconsolidator.h"
#include "geometryoffsettable.h"
#include "batchgenerator.h"

#include "rendersystem/private/rendermodel/rendermodelcontext.h"
#include "rendersystem/private/shaders/common/privateshaderdefs.h"

class OpenGLShaderProgram;

class GeometryUploader
{
public:
    GeometryUploader(const RenderModelContext& context,
                     RenderSystem::MaterialDefs::MaterialId materialId,
                     GeometryDataContainer& data,
                     GeometryOffsetTable& offsetTable,
                     OpenGLBufferCollection& buffers);

    bool uploadIfRequired();
    bool isValid() const;

    PrivateShaderDefs::ShaderId shaderIdWhenLastUploaded() const;

private:
    enum UploadFlags
    {
        NoUploadFlags = 0,

        MatricesUploadFlag = (1<<0),
        VerticesUploadFlag = (1<<1),

        AllUploadFlags = MatricesUploadFlag | VerticesUploadFlag
    };

    class InternalException : public QException
    {
    public:
        void raise() const override { throw *this; }
        InternalException* clone() const override { return new InternalException(*this); }
    };

    quint32 shouldUpload() const;
    quint32 checkForDirtyGeometry() const;
    bool uploadAllUniforms();
    bool uploadAllVertexData();
    void consolidateVerticesAndIndices();
    bool buffersCreated();
    void getShaderFromMaterial();

    quint32 calculateRequiredUniformBufferSize() const;
    quint32 calculateBatchSize(const BatchGenerator::GeometryDataVector& batch) const;
    void generateBatches();

    void prepareUniformBufferForUpload_x();
    void releaseUniformBuffer();
    void uploadUniformsInBatches();

    void uploadVertices_x();
    void uploadIndices_x();

    const RenderModelContext& m_Context;
    const RenderSystem::MaterialDefs::MaterialId m_nMaterialId;
    GeometryDataContainer& m_GeometryDataContainer;
    GeometryOffsetTable& m_OffsetTable;
    OpenGLBufferCollection& m_OpenGLBuffers;

    OpenGLShaderProgram* m_pCurrentShaderProgram;
    PrivateShaderDefs::ShaderId m_nShaderId;
    PrivateShaderDefs::ShaderId m_nShaderIdWhenLastUploaded;

    GeometryConsolidator m_Consolidator;
    BatchGenerator m_BatchGenerator;
    char* m_pUniformBufferData;
};

#endif // GEOMETRYUPLOADER_H
