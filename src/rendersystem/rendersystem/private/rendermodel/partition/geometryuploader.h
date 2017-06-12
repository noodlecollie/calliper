#ifndef GEOMETRYUPLOADER_H
#define GEOMETRYUPLOADER_H

#include <QSharedPointer>
#include <QException>

#include "openglbuffercollection.h"
#include "geometrydatacontainer.h"
#include "geometryconsolidator.h"
#include "geometryoffsettable.h"

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
        void raise() const { throw *this; }
        InternalException *clone() const { return new InternalException(*this); }
    };

    quint32 shouldUpload() const;
    quint32 checkForDirtyGeometry() const;
    bool uploadAllMatrices();
    bool uploadAllVertexData();
    void consolidateVerticesAndIndices();
    bool buffersCreated();
    void getShaderFromMaterial();

    void prepareUniformBufferForUpload_x();
    void releaseUniformBuffer();

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
    char* m_pUniformBufferData;
};

#endif // GEOMETRYUPLOADER_H
