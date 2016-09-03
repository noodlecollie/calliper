#ifndef RENDERMODELBATCH_H
#define RENDERMODELBATCH_H

#include "renderer_global.h"
#include <QOpenGLBuffer>
#include "rendermodelbatchparams.h"
#include "rendermodelbatchitem.h"
#include "shaders/shaderdefs.h"
#include "opengl/opengluniformbuffer.h"

class QOpenGLShaderProgram;

namespace NS_RENDERER
{
    class IShaderSpec;

    class RenderModelBatch
    {
    public:
        RenderModelBatch(QOpenGLBuffer::UsagePattern usagePattern, IShaderSpec* shaderSpec);
        ~RenderModelBatch();

        bool create();
        void destroy();

        void addItem(const RenderModelBatchParams &params);
        int itemCount() const;
        void clearItems();
        bool isFull() const;

        const IShaderSpec* shaderSpec() const;
        bool shaderSupportsBatching() const;

        void upload(bool force = false);
        bool needsUpload() const;

        // Assumes upload() has been called!
        void bindDraw();
        void setAttributePointers(QOpenGLShaderProgram* shaderProgram);
        void draw();
        void releaseDraw();

        int localPositionCount() const;
        int localNormalCount() const;
        int localColorCount() const;
        int localTextureCoordinateCount() const;
        int localIndexCount() const;

private:
        static void copyInIndexData(quint32* &dest, const quint32* source, int intCount);
        void copyInData(const RenderModelBatchParams &params, int vertexCount);
        int maxComponentsFromVertexSpec() const;
        void resizeAllBuffers(int numVertices);
        void uploadVertexData();
        void uploadUniformData();
        void writeToGlVertexBuffer(const QVector<float> &buffer, int &offset);
        void trySetAttributeBuffer(QOpenGLShaderProgram* shaderProgram, int &offset, ShaderDefs::VertexArrayAttribute attribute, int components, int count);
        void addIndices(const quint32* source, int count, int indexOffset);
        void addObjectIdsToPositions(int vertexOffset, int vertexCount, quint32 id);

        static inline quint32 batchIdMask(int numBits)
        {
            return (quint32)(~0) >> ((sizeof(quint32) * 8) - numBits);
        }

        QOpenGLBuffer::UsagePattern m_iUsagePattern;
        bool    m_bCreated;

        QOpenGLBuffer   m_GlVertexBuffer;
        QOpenGLBuffer   m_GlIndexBuffer;
        OpenGLUniformBuffer m_GlUniformBuffer;

        QVector<float>      m_LocalPositionBuffer;
        QVector<float>      m_LocalNormalBuffer;
        QVector<float>      m_LocalColorBuffer;
        QVector<float>      m_LocalTextureCoordinateBuffer;
        QVector<quint32>    m_LocalIndexBuffer;
        QList<QMatrix4x4>   m_ModelToWorldMatrices;

        QList<NS_RENDERER::RenderModelBatchItem> m_Items;

        const IShaderSpec*      m_pShaderSpec;
        bool                    m_bDataStale;
        const quint32           m_iBatchIdMask;
        GLuint                  m_iUniformBlockIndex;
    };
}

#endif // RENDERMODELBATCH_H
