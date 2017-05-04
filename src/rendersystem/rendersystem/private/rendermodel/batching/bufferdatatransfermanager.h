#ifndef BUFFERDATATRANSFERMANAGER_H
#define BUFFERDATATRANSFERMANAGER_H

#include <QHash>

#include "rendersystem/private/shaders/common/vertexformat.h"

#include "openglbuffercollection.h"
#include "bufferdatacontainer.h"

class BufferDataTransferManager
{
public:
    BufferDataTransferManager(OpenGLBufferCollection& buffers, BufferDataContainer& data, const VertexFormat& vertexFormat);

    bool ensureUploaded();

private:
    void insertBatchId(QVector<float>& vertexData, quint8 batchId, int offset, int stride);

    void consolidateVetexData(const QSharedPointer<ObjectSectionGeometryData>& objectGeometry,
                                     QVector<float>& vertexData, quint8 batchId);
    void consolidateIndexData(const QSharedPointer<ObjectSectionGeometryData>& objectGeometry,
                                     QVector<quint32>& indexData);

    void uploadVertexData(const QVector<float>& vertexData, int offsetInFloats);
    void uploadIndexData(const QVector<quint32>& indexData, int offsetInInts, quint32 indexIncrement);

    // Assumes OpenGL buffers are already created.
    void uploadData(int index);

    bool allocateAndMapBuffers();
    void unmapBuffers();

    void markAllObjectsDirty();
    bool anyDirtyObjectData() const;
    quint32 totalRequiredVertexBytes() const;
    quint32 totalRequiredIndexBytes() const;

    OpenGLBufferCollection& m_Buffers;
    BufferDataContainer& m_Data;
    const VertexFormat m_VertexFormat;

    int m_nVertexFloatsUploadedSoFar;
    int m_nIndexIntsUploadedSoFar;

    float* m_pVertexBufferPointer;
    quint32* m_pIndexBufferPointer;
};

#endif // BUFFERDATATRANSFERMANAGER_H
