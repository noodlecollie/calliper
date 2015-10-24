#include "cdebugcube.h"
#include "cvertexbundle.h"

static const float _unitCube_Position[] = {
    // X plane, positive normal
    0.5f, -0.5f, -0.5f,     // LL
    0.5f, 0.5f, -0.5f,      // LR
    0.5f, 0.5f, 0.5f,       // UR
    0.5f, -0.5f, 0.5f,      // UL

    // X plane, negative normal
    0.5f, 0.5f, -0.5f,      // LL
    0.5f, -0.5f, -0.5f,     // LR
    0.5f, -0.5f, 0.5f,      // UR
    0.5f, 0.5f, 0.5f,       // UL

    // Y plane, positive normal
    0.5f, 0.5f, -0.5f,      // LL
    -0.5f, 0.5f, -0.5f,     // LR
    -0.5f, 0.5f, 0.5f,      // UR
    0.5f, 0.5f, 0.5f,       // UL

    // Y plane, negative normal
    -0.5f, 0.5f, -0.5f,     // LL
    0.5f, 0.5f, -0.5f,      // LR
    0.5f, 0.5f, 0.5f,       // UR
    -0.5f, 0.5f, 0.5f,      // UL

    // Z plane, positive normal
    -0.5f, -0.5f, 0.5f,     // LL
    0.5f, -0.5f, 0.5f,      // LR
    0.5f, 0.5f, 0.5f,       // UR
    -0.5f, 0.5f, 0.5f,      // UL

    // Z plane, negative normal
    0.5f, -0.5f, 0.5f,      // LL
    -0.5f, -0.5f, 0.5f,     // LR
    -0.5f, 0.5f, 0.5f,      // UR
    0.5f, 0.5f, 0.5f,       // UL
};

static const float _unitCube_UV[] = {
    0,0,
    1,0,
    1,1,
    1,0,

    0,0,
    1,0,
    1,1,
    1,0,

    0,0,
    1,0,
    1,1,
    1,0,

    0,0,
    1,0,
    1,1,
    1,0,

    0,0,
    1,0,
    1,1,
    1,0,

    0,0,
    1,0,
    1,1,
    1,0,
};

CDebugCube::CDebugCube(CSceneObject *parent) : CSceneObject(parent)
{
    m_pVertexData = new CVertexBundle(this);

    for ( int i = 0; i < 4*6; i++ )
    {
        // Add data for each vertex.
        int pos = i*3;
        int uv = i*2;
        m_pVertexData->appendVertex(QVector3D(pos, pos+1, pos+2), QVector2D(uv, uv+1));
    }

    for (int i = 0; i < 6; i++)
    {
        // Add indices for each face.
        int base = i*4;
        m_pVertexData->appendIndex(base);
        m_pVertexData->appendIndex(base+1);
        m_pVertexData->appendIndex(base+2);

        m_pVertexData->appendIndex(base);
        m_pVertexData->appendIndex(base+2);
        m_pVertexData->appendIndex(base+3);
    }
}

const CVertexBundle* CDebugCube::vertexData() const
{
    return m_pVertexData;
}
