#ifndef UIMANIPULATOR_H
#define UIMANIPULATOR_H

#include "sceneobject.h"
#include <QList>
#include <QVector3D>
#include "callipermath.h"

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
    static void clampToNearestMultiple(QVector3D &vec, qint64 multiple, int axisFlags);

    explicit UIManipulator(SceneObject* parent = 0);

    virtual SceneObject* clone() const;

protected:
    explicit UIManipulator(const UIManipulator &cloneFrom);
};

#endif // UIMANIPULATOR_H
