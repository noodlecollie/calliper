#include "uimanipulator.h"

UIManipulator::UIManipulator(SceneObject *parent) : SceneObject(parent)
{

}

UIManipulator::UIManipulator(const UIManipulator &cloneFrom) : SceneObject(cloneFrom)
{

}

QList<QVector3D> UIManipulator::manipulationAxes(int axisFlags)
{
    QList<QVector3D> axes;

    if ( (axisFlags & AxisX) == AxisX )
    {
        axes.append(QVector3D(1,0,0));
    }

    if ( (axisFlags & AxisY) == AxisY )
    {
        axes.append(QVector3D(0,1,0));
    }

    if ( (axisFlags & AxisZ) == AxisZ )
    {
        axes.append(QVector3D(0,0,1));
    }

    return axes;
}

SceneObject* UIManipulator::clone() const
{
    return new UIManipulator(*this);
}

void UIManipulator::clampToNearestMultiple(QVector3D &vec, qint64 multiple, int axisFlags)
{
    for ( int i = 0; i < 3; i++ )
    {
        int flag = 1 << i;
        if ( (axisFlags & flag) != flag )
            continue;

        vec[i] = (float)Math::nearestMultiple(vec[i], multiple);
    }
}
