#include "cdebugcube.h"
#include "cvertexbundle.h"
#include <QVector2D>
#include <QUrl>

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

CDebugCube::CDebugCube(CSceneObject *parent) : CSceneObject(parent)
{
    m_pVertexData = new CVertexBundle(this);
    m_pVertexData->setInterleavingFormat(COpenGLRenderer::FormatPositionNormalUV);

    for ( int i = 0; i < 4*6; i++ )
    {
        // Add data for each vertex.
        int pos = i*3;
        int uv = i*2;
        
        // Calculate the normal.
        float xn = 0, yn = 0, zn = 0;
        switch ( i/4 )
        {
        case 0:
            xn = 1;
            break;
        case 1:
            xn = -1;
            break;
        case 2:
            yn = 1;
            break;
        case 3:
            yn = -1;
            break;
        case 4:
            zn = 1;
            break;
        case 5:
            zn = -1;
            break;
        }
        
        m_pVertexData->appendVertex(QVector3D(_unitCube_Position[pos], _unitCube_Position[pos+1], _unitCube_Position[pos+2]),
                                    QVector3D(xn,yn,zn),
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
    
    m_pVertexData->setTextureURI(QUrl("qrc:/test.png"));
}

CVertexBundle* CDebugCube::vertexData() const
{
    return m_pVertexData;
}
