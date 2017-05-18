#ifndef GEOMETRYUPLOADER_H
#define GEOMETRYUPLOADER_H

#include <QSharedPointer>
#include <QException>

#include "openglbuffercollection.h"
#include "geometrydatacontainer.h"

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
    bool ensureBuffersCreated();

    void prepareUniformBufferForUpload_x();
    void releaseUniformBuffer();

    void prepareVertexBufferForUpload_x();
    void releaseVertexBuffer();

    void prepareIndexBufferForUpload_x();
    void releaseIndexBuffer();

    void consolidateVertexData(QSharedPointer<GeometryData>& geometry, QVector<float>& vertexData);
    void consolidateIndexData(QSharedPointer<GeometryData>& geometry, QVector<quint32>& indexData);

    void consolidate(const QVector<QVector4D> &source,
                     QVector<float> &destination,
                     int components,
                     int offset,
                     int stride);

    quint32 computeTotalRequiredVertexBytes();
    quint32 computeTotalRequiredIndexBytes();

    GeometryDataContainer& m_GeometryDataContainer;
    OpenGLBufferCollection& m_OpenGLBuffers;
    PrivateShaderDefs::ShaderId m_nCurrentShaderId;

    bool m_bShaderChangedSinceLastUpload;
    OpenGLShaderProgram* m_pCurrentShaderProgram;
    quint8 m_nCurrentObjectId;
    quint32 m_nCurrentObjectIdMask;
    int m_nMaxBatchedItems;

    char* m_pUniformBufferData;
    float* m_pVertexBufferData;
    quint32* m_pIndexBufferData;
};

#endif // GEOMETRYUPLOADER_H
