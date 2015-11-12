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
#include "cprofilertimer.h"

// Size of each attribute in bytes.
static const int ATTRIBUTE_SIZE[] = {
    3 * sizeof(float),      // Position
    2 * sizeof(float),      // UV
    3 * sizeof(float),      // Normal
    3 * sizeof(float),      // Color
};

// Size of each interleaving format in bytes.
static const int FORMAT_SIZE[] = {
    ATTRIBUTE_SIZE[COpenGLRenderer::Position],
    ATTRIBUTE_SIZE[COpenGLRenderer::Position] + ATTRIBUTE_SIZE[COpenGLRenderer::UV],
    ATTRIBUTE_SIZE[COpenGLRenderer::Position] + ATTRIBUTE_SIZE[COpenGLRenderer::Normal] + ATTRIBUTE_SIZE[COpenGLRenderer::UV],
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
        
        case FormatPositionNormalUV:
        {
            switch (att)
            {
            case Position:
                return 0;
            case Normal:
                return 3 * sizeof(float);
            case UV:
                return 6 * sizeof(float);
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

void COpenGLRenderer::setAttributeLayouts(QOpenGLShaderProgram *p, InterleavingFormat format)
{
    for ( int i = 0; i < AttributeCount; i++ )
    {
        Attribute a = static_cast<Attribute>(i);
        int offset = attributeOffset(format, a);
        if ( offset < 0 ) continue;
        
        // This attribute exists within the format.
        p->setAttributeBuffer(a,
                              GL_FLOAT,
                              attributeOffset(format, a),
                              attributeSize(a)/sizeof(float),
                              interleavingFormatSize(format));
        p->enableAttributeArray(a);
    }
}

COpenGLRenderer::COpenGLRenderer(CResourceManager *resourceManager, QObject *parent) : QObject(parent)
{
    m_pResourceManager = resourceManager;
    m_colRenderColour = QColor(0,0,0);
}

COpenGLRenderer::~COpenGLRenderer()
{
}

void COpenGLRenderer::render(QOpenGLFunctions_3_2_Core *f, const CSceneObject *root, const CBasicCamera* camera)
{
    CProfilerTimer timer("COpenGLRenderer::render()");
    
    QMatrix4x4 projection = camera->lens()->projectionMatrix();
    QMatrix4x4 view = camera->worldToCamera();
    
    /*
    float deg = 30;
    float s45 = qSin(qRadiansToDegrees(deg));
    float c45 = qCos(qRadiansToDegrees(deg));
    QMatrix4x4 scaleDown(0.5f,0,0,0, 0,0.5f,0,0, 0,0,0.5f,0, 0,0,0,1);
    QMatrix4x4 rotateY(c45,0,s45,0, 0,1,0,0, -s45,0,c45,0, 0,0,0,1);
    QMatrix4x4 rotateX(1,0,0,0, 0,c45,-s45,0, 0,s45,c45,0, 0,0,0,1);
    QMatrix4x4 model = rotateX * rotateY * scaleDown;
    */
    
    QMatrix4x4 model = root->matrixLocalToParent();
    QMatrix4x4 modelViewProjection = projection * CBasicCamera::coordsHammerToOpenGL() * view * model;

    // Bind the vertex buffer we want to use.
    CVertexBundle* v = root->vertexData();
    v->upload();
    v->bindVertexBuffer(true);
    
    QOpenGLShaderProgram* p = m_pResourceManager->shader(v->shader());
    if ( !p )
    {
        p = m_pResourceManager->fallbackShader();
    }

    p->bind();

    setAttributeLayouts(p, v->interleavingFormat());
    p->setUniformValue("mat_MVP", modelViewProjection);
    p->setUniformValue("vec_Color", QVector4D(m_colRenderColour.redF(), m_colRenderColour.greenF(), m_colRenderColour.blueF(), m_colRenderColour.alphaF()));

    v->bindIndexBuffer(true);
    QOpenGLTexture* tex = m_pResourceManager->texture(v->textureURI());
    if ( !tex )
    {
        tex = m_pResourceManager->fallbackTexture();
    }
    tex->bind();
    
    f->glDrawElements(GL_TRIANGLES, v->indexCount(), GL_UNSIGNED_INT, (void*)0);

    p->release();
}

void COpenGLRenderer::renderChildren(QOpenGLFunctions_3_2_Core *f, const CSceneObject *root, const CBasicCamera *camera)
{
    QMatrix4x4 projection = camera->lens()->projectionMatrix();
    QMatrix4x4 view = camera->worldToCamera();
    
    QList<CSceneObject*> children = root->findChildren<CSceneObject*>(QString(), Qt::FindDirectChildrenOnly);
    qDebug() << "Number of objects to render:" << children.count();
    
    {
        CProfilerTimer timer("COpenGLRenderer::renderChildren()");
        foreach ( CSceneObject* o, children )
        {
            QMatrix4x4 model = o->matrixLocalToParent();
            QMatrix4x4 modelViewProjection = projection * CBasicCamera::coordsHammerToOpenGL() * view * model;
        
            // Bind the vertex buffer we want to use.
            CVertexBundle* v = o->vertexData();
            v->upload();
            v->bindVertexBuffer(true);
            
            QOpenGLShaderProgram* p = m_pResourceManager->shader(v->shader());
            if ( !p )
            {
                p = m_pResourceManager->fallbackShader();
            }
        
            p->bind();
        
            setAttributeLayouts(p, v->interleavingFormat());
            p->setUniformValue("mat_MVP", modelViewProjection);
            p->setUniformValue("vec_Color", QVector4D(m_colRenderColour.redF(), m_colRenderColour.greenF(), m_colRenderColour.blueF(), m_colRenderColour.alphaF()));
        
            v->bindIndexBuffer(true);
            QOpenGLTexture* tex = m_pResourceManager->texture(v->textureURI());
            if ( !tex )
            {
                tex = m_pResourceManager->fallbackTexture();
            }
            tex->bind();
            
            f->glDrawElements(GL_TRIANGLES, v->indexCount(), GL_UNSIGNED_INT, (void*)0);
        
            p->release();
        }
    }
}

QColor COpenGLRenderer::renderColor() const
{
    return m_colRenderColour;
}

void COpenGLRenderer::setRenderColor(const QColor &col)
{
    if ( col == m_colRenderColour ) return;
    
    m_colRenderColour = col;
}
