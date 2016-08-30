#ifndef RENDERMODELBATCH_H
#define RENDERMODELBATCH_H

#include "renderer_global.h"
#include <QObject>
#include <QOpenGLBuffer>
#include "rendermodelbatchparams.h"
#include "rendermodelbatchitem.h"
#include "shaderdefs.h"

class QOpenGLShaderProgram;

namespace NS_RENDERER
{
    class IShaderSpec;

    class RENDERERSHARED_EXPORT RenderModelBatch : public QObject
    {
        Q_OBJECT
    public:
        RenderModelBatch(QOpenGLBuffer::UsagePattern usagePattern, IShaderSpec* shaderSpec,
                         QOpenGLShaderProgram* shaderProgram, QObject* parent = 0);
        ~RenderModelBatch();

        bool create();
        void destroy();

        void addItem(const RenderModelBatchParams &params);
        int itemCount() const;
        void clearItems();

        const IShaderSpec* shaderSpec() const;
        QOpenGLShaderProgram* shaderProgram() const;

        void upload(bool force = false);
        bool needsUpload() const;

        // Assumes upload() has been called!
        void setAttributePointers();
        void draw();

        int localPositionCount() const;
        int localNormalCount() const;
        int localColorCount() const;
        int localTextureCoordinateCount() const;
        int localIndexCount() const;

        // Remove me once we're done testing!
        QOpenGLBuffer vertexBuffer() const { return m_GlVertexBuffer; }
        QOpenGLBuffer indexBuffer() const { return m_GlIndexBuffer; }
        GLuint vaoHandle() const;

private:
        static void copyInIndexData(quint32* &dest, const quint32* source, int intCount);
        void copyInData(const RenderModelBatchParams &params, int vertexCount);
        int maxComponentsFromVertexSpec() const;
        void resizeAllBuffers(int numVertices);
        void uploadVertexData();
        void writeToGlVertexBuffer(const QVector<float> &buffer, int &offset);
        void bindVAO();
        void trySetAttributeBuffer(int &offset, ShaderDefs::VertexArrayAttribute attribute, int components, int count);
        void addIndices(const quint32* source, int count, int indexOffset);

        GLuint  m_iVAOID;
        QOpenGLBuffer::UsagePattern m_iUsagePattern;
        bool    m_bCreated;

        QOpenGLBuffer   m_GlVertexBuffer;
        QOpenGLBuffer   m_GlIndexBuffer;

        QVector<float>      m_LocalPositionBuffer;
        QVector<float>      m_LocalNormalBuffer;
        QVector<float>      m_LocalColorBuffer;
        QVector<float>      m_LocalTextureCoordinateBuffer;
        QVector<quint32>    m_LocalIndexBuffer;
        QList<QMatrix4x4>   m_ModelToWorldMatrices;

        QList<NS_RENDERER::RenderModelBatchItem> m_Items;

        const IShaderSpec*      m_pShaderSpec;
        QOpenGLShaderProgram*   m_pShaderProgram;
        bool                    m_bDataStale;
    };
}

#endif // RENDERMODELBATCH_H
