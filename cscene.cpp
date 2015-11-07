#include "cscene.h"
#include "csceneobject.h"

CScene::CScene(QWidget *parent) : QWidget(parent)
{
    m_pRoot = new CSceneObject();
}

CScene::~CScene()
{
    // Scene objects shouldn't take a scene as a parent, so we manually delete our root.
    delete m_pRoot;
}

CSceneObject* CScene::root() const
{
    return m_pRoot;
}

