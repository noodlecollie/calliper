#include "uimanipulator.h"

UIManipulator::UIManipulator(SceneObject *parent) : SceneObject(parent)
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

