#include "copenglrenderer.h"
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QGuiApplication>
#include <QOpenGLShaderProgram>
#include <QtDebug>
#include <QMetaEnum>

#include <QOpenGLFunctions_3_2_Core>
#include "csceneobject.h"
#include "cbasiccamera.h"
#include <QMatrix4x4>
#include "cvertexbundle.h"
#include "ccameralens.h"
#include <QSurface>
#include <QtMath>
#include "cresourcemanager.h"

// Size of each attribute in bytes.
static const int ATTRIBUTE_SIZE[] = {
    3 * sizeof(float),      // Position
    2 * sizeof(float),      // UV
};

// Size of each interleaving format in bytes.
static const int FORMAT_SIZE[] = {
    ATTRIBUTE_SIZE[COpenGLRenderer::Position],
    ATTRIBUTE_SIZE[COpenGLRenderer::Position] + ATTRIBUTE_SIZE[COpenGLRenderer::UV],
};

// If the attribute is not present in the format, return -1.
int COpenGLRenderer::attributeOffset(InterleavingFormat format, Attribute att)
{
    switch (format)
    {
        case FormatPosition:
        {
            switch (att)
            {
            case Position:
                return 0;
            default:
                return -1;
            }
        }

        case FormatPositionUV:
        {
            switch(att)
            {
            case Position:
                return 0;
            case UV:
                return 3 * sizeof(float);
            default:
                return -1;
            }
        }

    default:
        return -1;
    }
}

int COpenGLRenderer::attributeSize(Attribute att)
{
    return ATTRIBUTE_SIZE[att];
}

int COpenGLRenderer::interleavingFormatSize(InterleavingFormat format)
{
    return FORMAT_SIZE[format];
}

COpenGLRenderer::COpenGLRenderer(CResourceManager *resourceManager, QObject *parent) : QObject(parent)
{
    m_pResourceManager = resourceManager;
}

COpenGLRenderer::~COpenGLRenderer()
{
}

void COpenGLRenderer::render(QOpenGLFunctions_3_2_Core *f, const CSceneObject *root, const CBasicCamera* camera)
{
    QMatrix4x4 projection = camera->lens()->projectionMatrix();
    QMatrix4x4 view = camera->worldToCamera();
    float deg = 30;
    float s45 = qSin(qRadiansToDegrees(deg));
    float c45 = qCos(qRadiansToDegrees(deg));
    QMatrix4x4 scaleDown(0.5f,0,0,0, 0,0.5f,0,0, 0,0,0.5f,0, 0,0,0,1);
    QMatrix4x4 rotateY(c45,0,s45,0, 0,1,0,0, -s45,0,c45,0, 0,0,0,1);
    QMatrix4x4 rotateX(1,0,0,0, 0,c45,-s45,0, 0,s45,c45,0, 0,0,0,1);
    QMatrix4x4 model = rotateX * rotateY * scaleDown;
    QMatrix4x4 modelViewProjection = projection * CBasicCamera::coordsHammerToOpenGL() * view * model;

    // Bind the vertex buffer we want to use.
    CVertexBundle* v = root->vertexData();
    v->upload();
    v->bindVertexBuffer(true);
    QOpenGLShaderProgram* p = m_pResourceManager->shader("SolidColour");
    Q_ASSERT(p);

    p->bind();

    p->setAttributeBuffer(COpenGLRenderer::Position,
                          GL_FLOAT,
                          COpenGLRenderer::attributeOffset(COpenGLRenderer::FormatPositionUV, COpenGLRenderer::Position),
                          COpenGLRenderer::attributeSize(COpenGLRenderer::Position)/sizeof(float),
                          COpenGLRenderer::interleavingFormatSize(COpenGLRenderer::FormatPositionUV));
    p->setUniformValue("mat_MVP", modelViewProjection);
    p->setUniformValue("vec_Color", QVector4D(1,0,0,1));
    p->enableAttributeArray(COpenGLRenderer::Position);

    v->bindIndexBuffer(true);
    f->glDrawElements(GL_TRIANGLES, v->indexCount(), GL_UNSIGNED_INT, (void*)0);

    p->release();
}
