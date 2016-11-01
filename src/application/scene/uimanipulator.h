#ifndef UIMANIPULATOR_H
#define UIMANIPULATOR_H

#include "sceneobject.h"
#include <QList>
#include <QVector3D>
#include "callipermath.h"

class UIManipulator : public SceneObject
{
	Q_OBJECT
    friend class BaseScene;
public:
    static QList<QVector3D> manipulationAxes(int axisFlags);
	static Math::AxisIdentifier planeConstraintAxis(int axisFlags, const HierarchicalObject &camera);

    // UI elements should not be serialised.
    virtual bool serialiseToJson(QJsonObject &) const;

    virtual ObjectFlags objectFlags() const;
    virtual void draw(ShaderStack *stack);

    static Math::AxisFlag axisFlagsFromPickColor(QRgb colour);
    static Math::AxisIdentifier axisIdentifierFromPickColor(QRgb colour);

protected:
    explicit UIManipulator(BaseScene* scene, SceneObject* parent);
    UIManipulator(const UIManipulator &cloneFrom);
    virtual ~UIManipulator();

    void initDefaults();

    const static QRgb PICKCOLOUR_X;
    const static QRgb PICKCOLOUR_Y;
    const static QRgb PICKCOLOUR_Z;
    const static QRgb PICKCOLOUR_XY;
    const static QRgb PICKCOLOUR_YZ;
    const static QRgb PICKCOLOUR_XZ;
    const static unsigned int PICKMASK;

    const static QRgb HOVERCOLOUR_X;
    const static QRgb HOVERCOLOUR_Y;
    const static QRgb HOVERCOLOUR_Z;
    const static QRgb HOVERCOLOUR_XY;
    const static QRgb HOVERCOLOUR_YZ;
    const static QRgb HOVERCOLOUR_XZ;

    QVector<int>    m_SectionsToDraw;
};

#endif // UIMANIPULATOR_H
