#ifndef RENDERBATCH_H
#define RENDERBATCH_H

#include <QHash>

#include "rendersystem/private/shaders/common/vertexformat.h"

#include "openglbuffercollection.h"
#include "bufferdatacontainer.h"

class RenderBatch
{
public:
    RenderBatch(const VertexFormat& vertexFormat,
                int maxBatches,
                int maxItemsPerBatch,
                QOpenGLBuffer::UsagePattern usagePattern);

    BufferDataContainer& data();
    const BufferDataContainer& data() const;

    bool ensureUploaded();
    void draw();

private:
    struct BatchMetadata
    {
        quint32 vertexOffsetBytes;
        quint32 vertexLengthBytes;

        quint32 indexOffsetBytes;
        quint32 indexLengthBytes;

        BatchMetadata(quint32 vOffset, quint32 vLength, quint32 iOffset, quint32 iLength)
            : vertexOffsetBytes(vOffset),
              vertexLengthBytes(vLength),
              indexOffsetBytes(iOffset),
              indexLengthBytes(iLength)
        {
        }

        BatchMetadata()
            : vertexOffsetBytes(0),
              vertexLengthBytes(0),
              indexOffsetBytes(0),
              indexLengthBytes(0)
        {
        }
    };

    template<typename T>
    void consolidate(const QVector<T> &source,
                     QVector<T> &destination,
                     int components,
                     int offset,
                     int stride,
                     bool addObjectId = false)
    {
        if ( stride <= 0 )
        {
            stride = 1;
        }

        for ( int srcItem = 0, destItem = offset; srcItem < source.count(); srcItem += components, destItem += stride)
        {
            for ( int curComponent = 0; curComponent < components; ++curComponent )
            {
                if ( addObjectId && curComponent == components - 1 )
                {
                    quint32 idToWrite = static_cast<quint32>(m_nCurrentObjectId) & m_nObjectIdMask;
                    quint32 value = static_cast<quint32>(source[srcItem + curComponent]) | idToWrite;

                    destination[destItem + curComponent] = static_cast<float>(value);
                }
                else
                {
                    destination[destItem + curComponent] = source[srcItem + curComponent];
                }
            }
        }
    }

    void consolidateVetexData(const QSharedPointer<ObjectSectionGeometryData>& objectGeometry,
                                     QVector<float>& vertexData);
    void consolidateIndexData(const QSharedPointer<ObjectSectionGeometryData>& objectGeometry,
                                     QVector<quint32>& indexData);

    void uploadVertexData(const QVector<float>& vertexData, int offsetInFloats);
    void uploadIndexData(const QVector<quint32>& indexData, int offsetInInts, quint32 indexIncrement);

    // Assumes OpenGL buffers are already created.
    void uploadBatchData(int batchIndex);
    void uploadData(int index);
    bool allocateAndMapBuffers();
    void unmapBuffers();

    void drawBatch(int index);

    void markAllObjectsDirty();
    bool anyDirtyObjectData() const;
    quint32 totalRequiredVertexBytes() const;
    quint32 totalRequiredIndexBytes() const;
    quint32 calculateObjectIdMask() const;

    OpenGLBufferCollection m_Buffers;
    BufferDataContainer m_Data;
    const VertexFormat m_VertexFormat;
    QVector<BatchMetadata> m_BatchMetadata;

    int m_nVertexFloatsUploadedSoFar;
    int m_nIndexIntsUploadedSoFar;
    float* m_pVertexBufferPointer;
    quint32* m_pIndexBufferPointer;

    const quint32 m_nObjectIdMask;
    quint8 m_nCurrentObjectId;
};

#endif // RENDERBATCH_H
