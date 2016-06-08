#include "axisdraghandle.h"

AxisDragHandle::AxisDragHandle(BaseScene *scene, SceneObject *parent, const QString &iconTexture) :
    BaseDragHandle(scene, parent, iconTexture)
{
    m_iAxisConstraints[0] = 0;
    m_iAxisConstraints[1] = 0;
    m_iAxisConstraints[2] = 0;
}

AxisDragHandle::AxisDragHandle(const AxisDragHandle &cloneFrom) : BaseDragHandle(cloneFrom)
{
    m_iAxisConstraints[0] = cloneFrom.m_iAxisConstraints[0];
    m_iAxisConstraints[1] = cloneFrom.m_iAxisConstraints[1];
    m_iAxisConstraints[2] = cloneFrom.m_iAxisConstraints[2];
}

AxisDragHandle::~AxisDragHandle()
{

}

const int* AxisDragHandle::axisConstraints() const
{
    return m_iAxisConstraints;
}

void AxisDragHandle::setAxisConstraints(int x, int y, int z)
{
    m_iAxisConstraints[0] = x;
    m_iAxisConstraints[1] = y;
    m_iAxisConstraints[2] = z;
}
