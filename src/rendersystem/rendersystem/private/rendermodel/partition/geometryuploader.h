#ifndef GEOMETRYUPLOADER_H
#define GEOMETRYUPLOADER_H

#include <QSharedPointer>
#include <QException>

#include "openglbuffercollection.h"
#include "geometrydatacontainer.h"
#include "geometryconsolidator.h"

#include "rendersystem/private/shaders/common/privateshaderdefs.h"

class OpenGLShaderProgram;

class GeometryUploader
{
public:
    GeometryUploader(GeometryDataContainer& data, OpenGLBufferCollection& buffers);

    PrivateShaderDefs::ShaderId currentShaderId() const;
    void setCurrentShaderId(PrivateShaderDefs::ShaderId shaderId);

    bool uploadIfRequired();

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
    bool ensureBuffersCreated();
    void setUpShaderDependentMembers();

    void prepareUniformBufferForUpload_x();
    void releaseUniformBuffer();

    void uploadVertices_x();
    void releaseVertexBuffer();

    void uploadIndices_x();
    void releaseIndexBuffer();

    GeometryDataContainer& m_GeometryDataContainer;
    OpenGLBufferCollection& m_OpenGLBuffers;
    PrivateShaderDefs::ShaderId m_nCurrentShaderId;

    bool m_bShaderChangedSinceLastUpload;
    OpenGLShaderProgram* m_pCurrentShaderProgram;

    GeometryConsolidator m_Consolidator;
    char* m_pUniformBufferData;
};

#endif // GEOMETRYUPLOADER_H
