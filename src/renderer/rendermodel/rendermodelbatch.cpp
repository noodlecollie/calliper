#include "rendermodelbatch.h"
#include "opengl/openglerrors.h"
#include "opengl/openglhelpers.h"
#include "shaders/ishaderspec.h"
#include <QOpenGLShaderProgram>

namespace NS_RENDERER
{
    RenderModelBatch::RenderModelBatch(QOpenGLBuffer::UsagePattern usagePattern, const VertexFormat &vertexFormat, int batchSize)
        : m_iUsagePattern(usagePattern),
          m_bCreated(false),
          m_GlVertexBuffer(QOpenGLBuffer::VertexBuffer),
          m_GlIndexBuffer(QOpenGLBuffer::IndexBuffer),
          m_GlUniformBuffer(QOpenGLBuffer::StaticDraw),
          m_VertexFormat(vertexFormat),
          m_iBatchSize(batchSize),
          m_bDataStale(false),
          m_iBatchIdMask(batchIdMask(bitsRequired(m_iBatchSize))),
          m_iUniformBlockIndex(0)
    {
    }

    RenderModelBatch::~RenderModelBatch()
    {
        destroy();
    }

    bool RenderModelBatch::create()
    {
        if ( m_bCreated )
            return true;

        m_GlVertexBuffer.setUsagePattern(m_iUsagePattern);
        m_GlIndexBuffer.setUsagePattern(m_iUsagePattern);
        m_GlUniformBuffer.setUsagePattern(m_iUsagePattern);

        m_bCreated = m_GlVertexBuffer.create() && m_GlIndexBuffer.create() && m_GlUniformBuffer.create();
        return m_bCreated;
    }

    void RenderModelBatch::destroy()
    {
        if ( !m_bCreated )
            return;

        m_GlVertexBuffer.destroy();
        m_GlIndexBuffer.destroy();
        m_GlUniformBuffer.destroy();

        m_bCreated = false;
    }

    void RenderModelBatch::upload(bool force)
    {
        if ( force || m_bDataStale )
        {
            uploadVertexData();
            uploadIndexData();
            uploadUniformData();

            m_bDataStale = false;
        }
    }

    bool RenderModelBatch::needsUpload() const
    {
        return m_bDataStale;
    }

    void RenderModelBatch::uploadVertexData()
    {
        // TODO
    }

    void RenderModelBatch::uploadIndexData()
    {
        // TODO
    }

    void RenderModelBatch::writeToGlVertexBuffer(const QVector<float> &buffer, int &offset)
    {
        m_GlVertexBuffer.write(offset, buffer.constData(), buffer.count() * sizeof(float));
        offset += buffer.count() * sizeof(float);
    }

    void RenderModelBatch::setAttributePointers(QOpenGLShaderProgram *shaderProgram)
    {
        Q_ASSERT_X(!m_bDataStale, Q_FUNC_INFO, "Data not uploaded before setting attribute pointers!");

        int offset = 0;

        trySetAttributeBuffer(shaderProgram,
                              offset,
                              ShaderDefs::PositionAttribute,
                              m_VertexFormat.positionComponents(),
                              /*TODO*/);

        trySetAttributeBuffer(shaderProgram,
                              offset,
                              ShaderDefs::NormalAttribute,
                              m_VertexFormat.normalComponents(),
                              /*TODO*/);

        trySetAttributeBuffer(shaderProgram,
                              offset,
                              ShaderDefs::ColorAttribute,
                              m_VertexFormat.colorComponents(),
                              /*TODO*/);

        trySetAttributeBuffer(shaderProgram,
                              offset,
                              ShaderDefs::TextureCoordinateAttribute,
                              m_VertexFormat.textureCoordinateComponents(),
                              /*TODO*/);
    }

    void RenderModelBatch::trySetAttributeBuffer(QOpenGLShaderProgram *shaderProgram, int &offset, ShaderDefs::VertexArrayAttribute attribute,
                                                 int components, int count)
    {
        if ( components > 0 )
        {
            shaderProgram->setAttributeBuffer(attribute,
                                                 GL_FLOAT,
                                                 offset * sizeof(GLfloat),
                                                 components);
            offset += count;
        }
    }

    void RenderModelBatch::bindDraw()
    {
        m_GlVertexBuffer.bind();
        m_GlIndexBuffer.bind();
    }

    void RenderModelBatch::draw()
    {
        GL_CURRENT_F;

        f->glDrawElements(GL_TRIANGLES, m_LocalIndexBuffer.count(), GL_UNSIGNED_INT, (void*)0);
    }

    void RenderModelBatch::releaseDraw()
    {
        m_GlIndexBuffer.release();
        m_GlVertexBuffer.release();
    }

    void RenderModelBatch::uploadUniformData()
    {
        // Not sure if this exact set of steps is required, but it's the
        // only way I got it to actually work.
        m_GlUniformBuffer.bind();
        m_GlUniformBuffer.allocate(m_ItemTable.count() * 16 * sizeof(float));
        m_GlUniformBuffer.release();

        m_GlUniformBuffer.bindToIndex(ShaderDefs::LocalUniformBlockBindingPoint);

        m_GlUniformBuffer.bind();
        for ( int i = 0; i < m_Items.count(); i++ )
        {
            m_GlUniformBuffer.write(i * 16 * sizeof(float), m_ModelToWorldMatrices.at(i).constData(), 16 * sizeof(float));
        }
        m_GlUniformBuffer.release();
    }

    bool RenderModelBatch::supportsBatching() const
    {
        return m_iBatchSize > 1;
    }

    bool RenderModelBatch::isFull() const
    {
        return m_Items.count() >= m_iBatchSize;
    }
}
