#ifndef UIMANIPULATOR_H
#define UIMANIPULATOR_H

#include "sceneobject.h"
#include <QList>
#include <QVector3D>

class UIManipulator : public SceneObject
{
public:
    enum ManipulationAxisFlag
    {
        AxisX = 0x1,
        AxisY = 0x2,
        AxisZ = 0x4,

        AxisXY = AxisX | AxisY,
        AxisXZ = AxisX | AxisZ,
        AxisYZ = AxisY | AxisZ,

        AxisXYZ = AxisX | AxisY | AxisZ
    };

    static QList<QVector3D> manipulationAxes(int axisFlags);

    UIManipulator(SceneObject* parent = 0);
};

#endif // UIMANIPULATOR_H
