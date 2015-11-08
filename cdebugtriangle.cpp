#include "cdebugtriangle.h"
#include "cvertexbundle.h"

static const float _triangle_position[] = {
    -1,-1,0,
    1,-1,0,
    0,1,0,
};

CDebugTriangle::CDebugTriangle(CSceneObject *parent) : CSceneObject(parent)
{
    m_pVertexData = new CVertexBundle(this);
    m_pVertexData->setInterleavingFormat(COpenGLRenderer::FormatPosition);
    
    m_pVertexData->appendVertex(QVector3D(-1,-1,0));
    m_pVertexData->appendVertex(QVector3D(1,-1,0));
    m_pVertexData->appendVertex(QVector3D(0,1,0));
    
    m_pVertexData->appendIndex(0);
    m_pVertexData->appendIndex(1);
    m_pVertexData->appendIndex(2);
    
    m_pVertexData->upload();
}

CVertexBundle* CDebugTriangle::vertexData() const
{
    return m_pVertexData;
}
