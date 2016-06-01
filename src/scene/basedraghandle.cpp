#include "basedraghandle.h"

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
