#include "cdebugcube.h"
#include "cvertexbundle.h"
#include <QVector2D>

static const float _unitCube_Position[] = {
    // X plane, positive normal
    0.5f, -0.5f, -0.5f,     // LL
    0.5f, 0.5f, -0.5f,      // LR
    0.5f, 0.5f, 0.5f,       // UR
    0.5f, -0.5f, 0.5f,      // UL

    // X plane, negative normal
    -0.5f, 0.5f, -0.5f,     // LL
    -0.5f, -0.5f, -0.5f,    // LR
    -0.5f, -0.5f, 0.5f,     // UR
    -0.5f, 0.5f, 0.5f,      // UL

    // Y plane, positive normal
    0.5f, 0.5f, -0.5f,      // LL
    -0.5f, 0.5f, -0.5f,     // LR
    -0.5f, 0.5f, 0.5f,      // UR
    0.5f, 0.5f, 0.5f,       // UL

    // Y plane, negative normal
    -0.5f, -0.5f, -0.5f,    // LL
    0.5f, -0.5f, -0.5f,     // LR
    0.5f, -0.5f, 0.5f,      // UR
    -0.5f, -0.5f, 0.5f,     // UL

    // Z plane, positive normal
    -0.5f, -0.5f, 0.5f,     // LL
    0.5f, -0.5f, 0.5f,      // LR
    0.5f, 0.5f, 0.5f,       // UR
    -0.5f, 0.5f, 0.5f,      // UL

    // Z plane, negative normal
    0.5f, -0.5f, -0.5f,     // LL
    -0.5f, -0.5f, -0.5f,    // LR
    -0.5f, 0.5f, -0.5f,     // UR
    0.5f, 0.5f, -0.5f,      // UL
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

const static float cube_vertices_interleaved[] = {
    -1.0f,-1.0f,-1.0f, 0,0,
    -1.0f,-1.0f, 1.0f, 1,0,
    -1.0f, 1.0f, 1.0f, 1,1,

    1.0f, 1.0f,-1.0f,  0,0,
    -1.0f,-1.0f,-1.0f, 1,0,
    -1.0f, 1.0f,-1.0f, 1,1,

    1.0f,-1.0f, 1.0f,  0,0,
    -1.0f,-1.0f,-1.0f, 1,0,
    1.0f,-1.0f,-1.0f,  1,1,

    1.0f, 1.0f,-1.0f,  0,0,
    1.0f,-1.0f,-1.0f,  1,1,
    -1.0f,-1.0f,-1.0f, 0,1,

    -1.0f,-1.0f,-1.0f, 0,0,
    -1.0f, 1.0f, 1.0f, 1,1,
    -1.0f, 1.0f,-1.0f, 0,1,

    1.0f,-1.0f, 1.0f,  0,0,
    -1.0f,-1.0f, 1.0f, 1,1,
    -1.0f,-1.0f,-1.0f, 0,1,

    -1.0f, 1.0f, 1.0f, 0,0,
    -1.0f,-1.0f, 1.0f, 1,0,
    1.0f,-1.0f, 1.0f,  1,1,

    1.0f, 1.0f, 1.0f, 0,0,
    1.0f,-1.0f,-1.0f, 1,0,
    1.0f, 1.0f,-1.0f, 1,1,

    1.0f,-1.0f,-1.0f, 0,0,
    1.0f, 1.0f, 1.0f, 1,1,
    1.0f,-1.0f, 1.0f, 0,1,

    1.0f, 1.0f, 1.0f,  0,0,
    1.0f, 1.0f,-1.0f,  1,0,
    -1.0f, 1.0f,-1.0f, 1,1,

    1.0f, 1.0f, 1.0f,  0,0,
    -1.0f, 1.0f,-1.0f, 1,1,
    -1.0f, 1.0f, 1.0f, 0,1,

    1.0f, 1.0f, 1.0f,  0,0,
    -1.0f, 1.0f, 1.0f, 1,1,
    1.0f,-1.0f, 1.0f,  0,1,
};

CDebugCube::CDebugCube(CSceneObject *parent) : CSceneObject(parent)
{
    m_pVertexData = new CVertexBundle(this);
    m_pVertexData->setInterleavingFormat(COpenGLRenderer::FormatPositionUV);

    for ( int i = 0; i < 4*6; i++ )
    {
        // Add data for each vertex.
        int pos = i*3;
        int uv = i*2;
        m_pVertexData->appendVertex(QVector3D(_unitCube_Position[pos], _unitCube_Position[pos+1], _unitCube_Position[pos+2]),
                                    QVector2D(_unitCube_UV[uv], _unitCube_UV[uv+1]));
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

    /*
    int count = sizeof(cube_vertices_interleaved)/(sizeof(float)*5);
    const float* arr = cube_vertices_interleaved;
    for (int i = 0; i < count; i++)
    {
        int ind = i*5;
        m_pVertexData->appendVertex(QVector3D(arr[ind], arr[ind+1], arr[ind+2]), QVector2D(arr[ind+3], arr[ind+4]));
        m_pVertexData->appendIndex(i);
    }
    */
}

const CVertexBundle* CDebugCube::vertexData() const
{
    return m_pVertexData;
}
