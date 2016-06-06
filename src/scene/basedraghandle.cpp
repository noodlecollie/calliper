#include "basedraghandle.h"
#include "basescene.h"

BaseDragHandle::BaseDragHandle(BaseScene *scene, SceneObject *parent, const QString &iconTexture) : UIManipulator(scene, parent)
{
    m_szIconTexture = iconTexture;
}

BaseDragHandle::BaseDragHandle(const BaseDragHandle &cloneFrom) : UIManipulator(cloneFrom)
{
    m_szIconTexture = cloneFrom.m_szIconTexture;
}

BaseDragHandle::~BaseDragHandle()
{

}

SceneObject* BaseDragHandle::clone() const
{
    return m_pScene->cloneSceneObject<BaseDragHandle>(this);
}
