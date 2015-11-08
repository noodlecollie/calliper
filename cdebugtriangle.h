#ifndef CDEBUGTRIANGLE_H
#define CDEBUGTRIANGLE_H

#include "csceneobject.h"

class CDebugTriangle : public CSceneObject
{
    Q_OBJECT
public:
    CDebugTriangle(CSceneObject* parent = 0);
    
    virtual CVertexBundle* vertexData() const;
    
private:
    CVertexBundle*  m_pVertexData;
};

#endif // CDEBUGTRIANGLE_H
