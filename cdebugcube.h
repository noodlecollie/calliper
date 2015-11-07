#ifndef CDEBUGCUBE_H
#define CDEBUGCUBE_H

#include "csceneobject.h"

class CVertexBundle;

class CDebugCube : public CSceneObject
{
    Q_OBJECT
public:
    CDebugCube(CSceneObject* parent = 0);

    virtual CVertexBundle* vertexData() const;

private:
    CVertexBundle*  m_pVertexData;
};

#endif // CDEBUGCUBE_H
