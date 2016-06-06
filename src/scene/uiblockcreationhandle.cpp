#include "uiblockcreationhandle.h"
#include "basescene.h"
#include "geometryfactory.h"
#include "shaders.h"
#include <QtMath>

static const QRgb scaleHandleColours[6] =
{
    0xffff0000,
    0xff00ff00,
    0xff0000ff,
    0xffff0000,
    0xff00ff00,
    0xff0000ff,
};

UIBlockCreationHandle::UIBlockCreationHandle(BaseScene *scene, SceneObject *parent) : UIManipulator(scene, parent)
{
    initDefaults();
}

UIBlockCreationHandle::~UIBlockCreationHandle()
{

}

UIBlockCreationHandle::UIBlockCreationHandle(const UIBlockCreationHandle &cloneFrom) : UIManipulator(cloneFrom)
{
    initDefaults();
    m_Bounds = cloneFrom.m_Bounds;
}

void UIBlockCreationHandle::initDefaults()
{
    m_bRebuildGeometry = true;
    memset(m_pScaleHandles, 0, 6 * sizeof(SceneObject*));
    createScaleHandles();
}

BoundingBox UIBlockCreationHandle::bounds() const
{
    return m_Bounds;
}

void UIBlockCreationHandle::setBounds(const BoundingBox &bounds)
{
    if ( bounds == m_Bounds )
        return;

    m_Bounds = bounds;
    m_bRebuildGeometry = true;
}

void UIBlockCreationHandle::setBounds(const QVector3D &min, const QVector3D &max)
{
    setBounds(BoundingBox(min,max));
}

void UIBlockCreationHandle::rebuildGeometry()
{
    if ( geometryCount() > 0 )
    {
        clearGeometry();
    }

    GeometryData* geom = GeometryFactory::lineCuboid(m_Bounds, QColor::fromRgb(0xffffffff));
    geom->setShaderOverride(PerVertexColorShader::staticName());
    appendGeometry(geom);

    updateScaleHandles();

    m_bRebuildGeometry = false;
}

void UIBlockCreationHandle::draw(ShaderStack *stack)
{
    if ( m_bRebuildGeometry )
        rebuildGeometry();

    SceneObject::draw(stack);
}

void UIBlockCreationHandle::createScaleHandles()
{
    QMatrix4x4 transform;

    for ( int i = 0; i < 6; i++ )
    {
        UIManipulator* obj = m_pScene->createSceneObject<UIManipulator>(m_pScene->root());

        switch (i)
        {
            case 1:     // Y+
            {
                transform = Math::matrixRotateZ(qDegreesToRadians(90.0f));
                obj->setObjectName("_blockScaleYPos");
                break;
            }

            case 2:     // Z+
            {
                transform = Math::matrixRotateY(qDegreesToRadians(-90.0f));
                obj->setObjectName("_blockScaleZPos");
                break;
            }

            case 3:     // X-
            {
                transform = Math::matrixRotateZ(qDegreesToRadians(180.0f));
                obj->setObjectName("_blockScaleXNeg");
                break;
            }

            case 4:     // Y-
            {
                transform = Math::matrixRotateZ(qDegreesToRadians(-90.0f));
                obj->setObjectName("_blockScaleYNeg");
                break;
            }

            case 5:     // Z-
            {
                transform = Math::matrixRotateY(qDegreesToRadians(90.0f));
                obj->setObjectName("_blockScaleZNeg");
                break;
            }

            default:    // X+
            {
                transform = QMatrix4x4();
                obj->setObjectName("_blockScaleXPos");
                break;
            }
        }

        GeometryData* geometry = GeometryFactory::coneSolidColor(0.025f, 0.25f, 12, QColor::fromRgb(scaleHandleColours[i]));
        geometry->transform(transform);
        obj->appendGeometry(geometry);
        obj->setHidden(true);

        m_pScaleHandles[i] = obj;
    }
}

void UIBlockCreationHandle::updateScaleHandles()
{
    if ( m_Bounds.hasZeroVolume() )
    {
        hideAllScaleHandles();
        return;
    }

    QVector3D radius = (m_Bounds.max() - m_Bounds.min())/2.0f;
    QVector3D centroid = m_Bounds.centroid();

    for ( int i = 0; i < 6; i++ )
    {
        QVector3D delta = radius;
        switch (i%3)
        {
            case 0:
                delta.setY(0);
                delta.setZ(0);
                break;

            case 1:
                delta.setX(0);
                delta.setZ(0);
                break;

            case 2:
                delta.setX(0);
                delta.setY(0);
                break;

            default:
                break;
        }

        if ( i >= 3 )
            delta *= -1;

        m_pScaleHandles[i]->setHidden(false);
        m_pScaleHandles[i]->setPosition(centroid + delta);
    }
}

void UIBlockCreationHandle::hideAllScaleHandles()
{
    for ( int i = 0; i < 6; i++)
        m_pScaleHandles[i]->setHidden(true);
}
