#ifndef CVERTEXBUNDLE_H
#define CVERTEXBUNDLE_H

#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QList>
#include "copenglrenderer.h"
#include <QOpenGLBuffer>
#include <QUrl>

class CVertexBundle : public QObject
{
    Q_OBJECT
public:
    explicit CVertexBundle(QObject *parent = 0);
    ~CVertexBundle();

    // Setting the interleaving format does not resize the vector.
    COpenGLRenderer::InterleavingFormat interleavingFormat() const;
    void setInterleavingFormat(COpenGLRenderer::InterleavingFormat format);

    void clear();
    void clearVertices();
    void clearIndices();

    // Number of vertices (ie. groups of attributes).
    int vertexCount() const;
    // Size of the number of vertices stored, in bytes.
    unsigned int vertexDataSize() const;

    // Number of indices. If 0, indices are not used.
    int indexCount() const;
    // Size of the number of indices stored, in bytes.
    unsigned int indexDataSize() const;

    void appendVertex(const QVector3D &position);
    void appendVertex(const QVector3D &position, const QVector2D &uv);
    void appendVertex(const QVector3D &position, const QVector3D &normal, const QVector2D &uv);

    void appendIndex(unsigned int idx);

    void setVertexPosition(int vertex, const QVector3D &position);
    void setVertexUV(int vertex, const QVector2D &uv);
    void setVertexNormal(int vertex, const QVector3D &normal);

    void setIndex(int position, unsigned int value);

    //float* vertexData();
    const float* vertexConstData() const;

    //unsigned int* indexData();
    const unsigned int* indexConstData() const;

    // If dirty, data needs to be reuploaded.
    bool isVertexDataDirty() const;
    bool isIndexDataDirty() const;
    void upload(bool force = false);

    // Assumes buffer has been uploaded (which will have created it).
    // Passing true binds, passing false releases.
    bool bindVertexBuffer(bool bind);
    bool bindIndexBuffer(bool bind);
    
    QString shader() const;
    void setShader(const QString &name);
    
    QUrl textureURI() const;
    void setTextureURI(const QUrl &uri);

signals:

public slots:

private:
    int floatsPerVertex() const;
    int incrementSize();
    void createBuffer(QOpenGLBuffer &buffer);
    void uploadData(QOpenGLBuffer &buffer, const void* data, int size);

    QVector<float>          m_VertexData;
    QVector<unsigned int>   m_IndexData;
    COpenGLRenderer::InterleavingFormat      m_iInterleavingFormat;
    QOpenGLBuffer   m_VertexBuffer;
    QOpenGLBuffer   m_IndexBuffer;
    bool    m_bVBufferDirty;
    bool    m_bIBufferDirty;
    QString m_ShaderName;
    QUrl    m_TextureUri;
};

#endif // CVERTEXBUNDLE_H
