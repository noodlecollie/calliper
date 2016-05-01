#include "uimanipulator.h"
#include "shaderstack.h"

const QRgb UIManipulator::PICKCOLOUR_X = 0x88ff0000;
const QRgb UIManipulator::PICKCOLOUR_Y = 0x8800ff00;
const QRgb UIManipulator::PICKCOLOUR_Z = 0x880000ff;
const QRgb UIManipulator::PICKCOLOUR_XY = 0x88888800;
const QRgb UIManipulator::PICKCOLOUR_YZ = 0x88008888;
const QRgb UIManipulator::PICKCOLOUR_XZ = 0x88880088;
const unsigned int UIManipulator::PICKMASK = 0x00ffffff;

UIManipulator::UIManipulator(SceneObject *parent) : SceneObject(parent)
{

}

UIManipulator::UIManipulator(const UIManipulator &cloneFrom) : SceneObject(cloneFrom)
{

}

UIManipulator::~UIManipulator()
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

Math::AxisIdentifier UIManipulator::planeConstraintAxis(int axisFlags, const HierarchicalObject &camera)
{
	switch (axisFlags)
	{
		// If the axis is single, we want to choose the plane depending on which one the camera is most perpendicular to.
		case AxisX:
		{
			QVector3D dir = Math::angleToVectorSimple(camera.angles());

			// Choose the plane whose normal most closely matches the camera's view direction
			if ( qAbs(QVector3D::dotProduct(dir, QVector3D(0,1,0))) > qAbs(QVector3D::dotProduct(dir, QVector3D(0,0,1))) )
			{
				return Math::AxisY;
			}
			else
			{
				return Math::AxisZ;
			}
		}

		case AxisY:
		{
			QVector3D dir = Math::angleToVectorSimple(camera.angles());

			// Choose the plane whose normal most closely matches the camera's view direction
			if ( qAbs(QVector3D::dotProduct(dir, QVector3D(1,0,0))) > qAbs(QVector3D::dotProduct(dir, QVector3D(0,0,1))) )
			{
				return Math::AxisX;
			}
			else
			{
				return Math::AxisZ;
			}
		}

		case AxisZ:
		{
			QVector3D dir = Math::angleToVectorSimple(camera.angles());

			// Choose the plane whose normal most closely matches the camera's view direction
			if ( qAbs(QVector3D::dotProduct(dir, QVector3D(1,0,0))) > qAbs(QVector3D::dotProduct(dir, QVector3D(0,1,0))) )
			{
				return Math::AxisX;
			}
			else
			{
				return Math::AxisY;
			}
		}

		// Otherwise, return the axis not in the plane.
		case AxisXY:
		{
			return Math::AxisZ;
		}

		case AxisXZ:
		{
			return Math::AxisY;
		}

		case AxisYZ:
		{
			return Math::AxisX;
		}

		default:
		{
			Q_ASSERT(false);
			return Math::AxisX;
		}
	}
}

bool UIManipulator::serialiseToJson(QJsonObject &) const
{
    // UI elements should not be serialised.
    return false;
}

bool UIManipulator::editable() const
{
    return false;
}

void UIManipulator::draw(ShaderStack *stack)
{
    if ( isEmpty() )
        return;

    QVector4D testVec = stack->cameraProjectionTop() * stack->coordinateTransformTop() * stack->worldToCameraTop()
            * stack->modelToWorldTop() * QVector4D(0,0,0,1);

    stack->counterScalePush();
    stack->counterScaleSetTop(testVec.z());

    for ( int i = 0; i < geometryCount(); i++ )
    {
        drawGeometry(geometryAt(i).data(), stack);
    }

    stack->counterScalePop();
}

int UIManipulator::axisFlagsFromPickColor(QRgb colour)
{
    switch (colour & PICKMASK)
    {
        case PICKCOLOUR_X & PICKMASK:
            return UIManipulator::AxisX;

        case PICKCOLOUR_Y & PICKMASK:
            return UIManipulator::AxisY;

        case PICKCOLOUR_Z & PICKMASK:
            return UIManipulator::AxisZ;

        case PICKCOLOUR_XY & PICKMASK:
            return UIManipulator::AxisXY;

        case PICKCOLOUR_XZ & PICKMASK:
            return UIManipulator::AxisXZ;

        case PICKCOLOUR_YZ & PICKMASK:
            return UIManipulator::AxisYZ;

        default:
            return 0;
    }
}
