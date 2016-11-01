#ifndef AXISDRAGHANDLE_H
#define AXISDRAGHANDLE_H

#include "basedraghandle.h"
#include "callipermath.h"

class AxisDragHandle : public BaseDragHandle
{
    Q_OBJECT
    friend class BaseScene;
public:
    const int* axisConstraints() const;
    void setAxisConstraints(int x, int y, int z);

protected:
    explicit AxisDragHandle(BaseScene* scene, SceneObject *parent, const QString &iconTexture);
    AxisDragHandle(const AxisDragHandle &cloneFrom);
    virtual ~AxisDragHandle();

private:
    int     m_iAxisConstraints[3];
};

#endif // AXISDRAGHANDLE_H
