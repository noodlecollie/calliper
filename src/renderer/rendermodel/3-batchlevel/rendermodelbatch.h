#ifndef RENDERMODELBATCH_H
#define RENDERMODELBATCH_H

#include "renderer_global.h"
#include <QOpenGLBuffer>
#include "rendermodelbatchparams.h"
#include "rendermodel/4-batchitemlevel/matrixbatchitem.h"
#include "shaders/shaderdefs.h"
#include "opengl/opengluniformbuffer.h"
#include "shaders/vertexformat.h"
#include <QScopedArrayPointer>
#include "general/hashfunctions.h"
#include "rendermodel/4-batchitemlevel/rendermodelbatchitemkey.h"

class QOpenGLShaderProgram;

namespace NS_RENDERER
{
    class IShaderSpec;

    class RenderModelBatch
    {
    public:
        RenderModelBatch(QOpenGLBuffer::UsagePattern usagePattern, const VertexFormat &vertexFormat, int batchSize);
        ~RenderModelBatch();

        bool create();
        void destroy();

        bool supportsBatching() const;
        bool canAddNewItem(const QMatrix4x4 &key) const;

        // If this batch has reached max size and the data requires a new item, add() does nothing.
        bool add(const RenderModelBatchParams &params);
        void clear();
        bool containsMatrix(const QMatrix4x4 &mat) const;

        void upload(bool force = false);
        void upload(QVector<float> &v, QVector<float> &u, QVector<quint32> &i);

        void bindDraw();
        void setAttributePointers(QOpenGLShaderProgram* program);
        void draw();
        void releaseDraw();

private:
        static inline quint32 batchIdMask(int numBits)
        {
            return (quint32)(~0) >> ((sizeof(quint32) * 8) - numBits);
        }

        static inline quint8 bitsRequired(int maxValue)
        {
            for ( quint32 i = 0; i < sizeof(quint32)*8; i++ )
            {
                if ( 1 << i >= maxValue )
                {
                    return i+1;
                }
            }

            return sizeof(quint32)*8;
        }

        void uploadVertexData(const QVector<float> &source, int &offsetBytes);
        void uploadVertexDataTemp(const QVector<float> &source, QVector<float> &dest, int &offsetBytes);
        int getVertexDataCountInBytes() const;
        int getIndexDataCountInBytes() const;
        void uploadVertexData();
        void uploadVertexDataTemp(QVector<float> &dest);
        void uploadIndexData();
        void uploadIndexDataTemp(QVector<quint32> &dest);
        void uploadUniformData();
        void uploadUniformDataTemp(QVector<float> &dest);
        void addObjectIdsToPositions(RenderModelBatchItem* item, int floatOffset, int floatCount);
        void addObjectIdsToPositions(RenderModelBatchItem *item);
        quint32 getNextObjectId();
        void invalidateObjectId(quint32 id);
        void trySetAttributeBuffer(QOpenGLShaderProgram *shaderProgram, int &offset,
                                   ShaderDefs::VertexArrayAttribute attribute,
                                   int components, int count);

        QOpenGLBuffer::UsagePattern m_iUsagePattern;
        bool                        m_bCreated;

        QOpenGLBuffer   m_GlVertexBuffer;
        QOpenGLBuffer   m_GlIndexBuffer;
        OpenGLUniformBuffer m_GlUniformBuffer;

        VertexFormat            m_VertexFormat;
        int                     m_iBatchSize;
        bool                    m_bDataStale;
        const quint32           m_iBatchIdMask;
        GLuint                  m_iUniformBlockIndex;

        QScopedArrayPointer<bool>   m_bUsedObjectIds;

        QHash<RenderModelBatchItemKey, RenderModelBatchItem*> m_ItemTable;
        QHash<QMatrix4x4, quint32> m_MatricesUsed;

        struct UploadMetadata
        {
            int numPositions;
            int numNormals;
            int numColors;
            int numTexCoords;
            int numIndices;
        };
        UploadMetadata m_UploadMetadata;
        void clearUploadMetadata();
    };
}

#endif // RENDERMODELBATCH_H
