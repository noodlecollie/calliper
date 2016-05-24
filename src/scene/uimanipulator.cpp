#include "uimanipulator.h"
#include "shaderstack.h"
#include "basescene.h"

const QRgb UIManipulator::PICKCOLOUR_X = 0x88ff0000;
const QRgb UIManipulator::PICKCOLOUR_Y = 0x8800ff00;
const QRgb UIManipulator::PICKCOLOUR_Z = 0x880000ff;
const QRgb UIManipulator::PICKCOLOUR_XY = 0x88888800;
const QRgb UIManipulator::PICKCOLOUR_YZ = 0x88008888;
const QRgb UIManipulator::PICKCOLOUR_XZ = 0x88880088;
const unsigned int UIManipulator::PICKMASK = 0x00ffffff;

const QRgb UIManipulator::HOVERCOLOUR_X = 0x88f7a59b;
const QRgb UIManipulator::HOVERCOLOUR_Y = 0x889bd092;
const QRgb UIManipulator::HOVERCOLOUR_Z = 0x8893b3de;
const QRgb UIManipulator::HOVERCOLOUR_XY = 0x88fec67d;
const QRgb UIManipulator::HOVERCOLOUR_YZ = 0x8899d5c9;
const QRgb UIManipulator::HOVERCOLOUR_XZ = 0x88c48cbe;

UIManipulator::UIManipulator(BaseScene *scene, SceneObject *parent) : SceneObject(scene, parent)
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

    if ( (axisFlags & Math::AxisFlagX) == Math::AxisFlagX )
    {
        axes.append(QVector3D(1,0,0));
    }

    if ( (axisFlags & Math::AxisFlagY) == Math::AxisFlagY )
    {
        axes.append(QVector3D(0,1,0));
    }

    if ( (axisFlags & Math::AxisFlagZ) == Math::AxisFlagZ )
    {
        axes.append(QVector3D(0,0,1));
    }

    return axes;
}

SceneObject* UIManipulator::clone() const
{
    return m_pScene->cloneSceneObject<UIManipulator>(this);
}

Math::AxisIdentifier UIManipulator::planeConstraintAxis(int axisFlags, const HierarchicalObject &camera)
{
	switch (axisFlags)
	{
		// If the axis is single, we want to choose the plane depending on which one the camera is most perpendicular to.
        case Math::AxisFlagX:
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

        case Math::AxisFlagY:
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

        case Math::AxisFlagZ:
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
        case Math::AxisFlagXY:
		{
			return Math::AxisZ;
		}

        case Math::AxisFlagXZ:
		{
			return Math::AxisY;
		}

        case Math::AxisFlagYZ:
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

Math::AxisFlag UIManipulator::axisFlagsFromPickColor(QRgb colour)
{
    switch (colour & PICKMASK)
    {
        case PICKCOLOUR_X & PICKMASK:
            return Math::AxisFlagX;

        case PICKCOLOUR_Y & PICKMASK:
            return Math::AxisFlagY;

        case PICKCOLOUR_Z & PICKMASK:
            return Math::AxisFlagZ;

        case PICKCOLOUR_XY & PICKMASK:
            return Math::AxisFlagXY;

        case PICKCOLOUR_XZ & PICKMASK:
            return Math::AxisFlagXZ;

        case PICKCOLOUR_YZ & PICKMASK:
            return Math::AxisFlagYZ;

        default:
            return Math::NoAxisFlag;
    }
}

Math::AxisIdentifier UIManipulator::axisIdentifierFromPickColor(QRgb colour)
{
    switch (colour & PICKMASK)
    {
        case PICKCOLOUR_X & PICKMASK:
            return Math::AxisX;

        case PICKCOLOUR_Y & PICKMASK:
            return Math::AxisY;

        case PICKCOLOUR_Z & PICKMASK:
            return Math::AxisZ;

        case PICKCOLOUR_XY & PICKMASK:
            return Math::AxisXY;

        case PICKCOLOUR_XZ & PICKMASK:
            return Math::AxisXZ;

        case PICKCOLOUR_YZ & PICKMASK:
            return Math::AxisYZ;

        default:
            return Math::NoAxis;
    }
}
