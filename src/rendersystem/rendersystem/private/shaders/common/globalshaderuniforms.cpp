#include "globalshaderuniforms.h"

#include "rendersystem/private/opengl/openglerrors.h"
#include "rendersystem/private/shaders/common/privateshaderdefs.h"

namespace
{
    const int DATA_SIZE = (2 * 16 * sizeof(float)) + (3 * sizeof(float));

    int copy(OpenGLUniformBuffer &dest, const char* source, int offset, int byteCount)
    {
        GLTRY(dest.write(offset, source, byteCount));
        return byteCount;
    }

    int copy(OpenGLUniformBuffer &dest, const QVector3D& vec, int offset)
    {
        float temp[3] = { vec.x(), vec.y(), vec.z() };
        return copy(dest, reinterpret_cast<const char*>(temp), offset, 3 * sizeof(float));
    }

    inline int copy(OpenGLUniformBuffer &dest, const QMatrix4x4 &mat, int offset)
    {
        return copy(dest, reinterpret_cast<const char*>(mat.constData()), offset, 16 * sizeof(float));
    }
}

GlobalShaderUniforms::GlobalShaderUniforms(QOpenGLBuffer::UsagePattern usagePattern)
    : m_bNeedsUpload(true), m_UniformBuffer(usagePattern),
      m_matWorldToCamera(), m_matProjection(),
      m_vecDirectionalLight(QVector3D(1,1,1).normalized())
{
}

GlobalShaderUniforms::~GlobalShaderUniforms()
{
    destroy();
}

void GlobalShaderUniforms::create()
{
    if ( m_UniformBuffer.isCreated() )
        return;

    m_UniformBuffer.create();
}

void GlobalShaderUniforms::destroy()
{
    if ( !m_UniformBuffer.isCreated() )
        return;

    m_UniformBuffer.destroy();
}

QMatrix4x4 GlobalShaderUniforms::worldToCameraMatrix() const
{
    return m_matWorldToCamera;
}

void GlobalShaderUniforms::setWorldToCameraMatrix(const QMatrix4x4 &mat)
{
    if ( m_matWorldToCamera == mat )
        return;

    m_matWorldToCamera = mat;
    m_bNeedsUpload = true;
}

QMatrix4x4 GlobalShaderUniforms::projectionMatrix() const
{
    return m_matProjection;
}

void GlobalShaderUniforms::setProjectionMatrix(const QMatrix4x4 &mat)
{
    if ( m_matProjection == mat )
        return;

    m_matProjection = mat;
    m_bNeedsUpload = true;
}

QVector3D GlobalShaderUniforms::directionalLight() const
{
    return m_vecDirectionalLight;
}

void GlobalShaderUniforms::setDirectionalLight(const QVector3D &vec)
{
    QVector3D temp = vec.normalized();
    if ( m_vecDirectionalLight == temp )
        return;

    m_vecDirectionalLight = temp;
    m_bNeedsUpload = true;
}

bool GlobalShaderUniforms::needsUpload() const
{
    return m_bNeedsUpload;
}

void GlobalShaderUniforms::upload()
{
    if ( !m_bNeedsUpload )
        return;

    GLTRY(m_UniformBuffer.bind());
    GLTRY(m_UniformBuffer.allocate(DATA_SIZE));
    GLTRY(m_UniformBuffer.release());

    GLTRY(m_UniformBuffer.bindToIndex(PrivateShaderDefs::GlobalUniformBlockBindingPoint));

    GLTRY(m_UniformBuffer.bind());
    int offset = 0;

    offset += copy(m_UniformBuffer, m_matWorldToCamera, offset);
    offset += copy(m_UniformBuffer, m_matProjection, offset);
    offset += copy(m_UniformBuffer, m_vecDirectionalLight, offset);

    GLTRY(m_UniformBuffer.release());

    m_bNeedsUpload = false;
}
