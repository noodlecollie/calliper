#include "uiblockcreationhandle.h"
#include "basescene.h"
#include "geometryfactory.h"
#include "shaders.h"
#include <QtMath>
#include "axisdraghandle.h"

#define TEXTURE_SQUARE_HANDLE "/textures/ui/square-handle"

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
    memset(m_pScaleHandles, 0, SCALE_HANDLE_COUNT * sizeof(AxisDragHandle*));
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
    for ( int i = 0; i < SCALE_HANDLE_COUNT; i++ )
    {
        // 0-7:   Z max handles, starting at min on X and Y and proceeding anticlockwise.
        // 8-15:  Same but Z min.
        // 16-19: Edges connecting each corner.

        int x = 0, y = 0, z = 0;

        // Choose X constraint:
        switch (i)
        {
            // +X:
            case 2:
            case 3:
            case 4:
            case 10:
            case 11:
            case 12:
            case 17:
            case 18:
                x = 1;
                break;

            // -X:
            case 0:
            case 6:
            case 7:
            case 8:
            case 14:
            case 15:
            case 16:
            case 19:
                x = -1;
                break;

            default:
                break;
        }

        // Choose Y constraint:
        switch(i)
        {
            // +Y:
            case 4:
            case 5:
            case 6:
            case 12:
            case 13:
            case 14:
            case 18:
            case 19:
                y = 1;
                break;

            // -Y:
            case 0:
            case 1:
            case 2:
            case 8:
            case 9:
            case 10:
            case 16:
            case 17:
                y = -1;
                break;

            default:
                break;
        }

        // Choose Z constraint:
        switch(i)
        {
            // +Z:
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                z = 1;
                break;

            // -Z:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
                z = -1;
                break;

            default:
                break;
        }

        AxisDragHandle* obj = m_pScene->createSceneObject<AxisDragHandle>(m_pScene->generalObjectsNode(), TEXTURE_SQUARE_HANDLE);
        obj->setObjectName(QString("_blockScaleHandle%0%1%2").arg(x).arg(y).arg(z));
        obj->setAxisConstraints(x,y,z);
        obj->setHidden(true);
        obj->setScale(0.1f);

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

    for ( int i = 0; i < SCALE_HANDLE_COUNT; i++ )
    {
        QVector3D delta = radius;

        const int* constraints = m_pScaleHandles[i]->axisConstraints();
        for ( int i = 0; i < 3; i++ )
        {
            delta[i] *= constraints[i];
        }

        m_pScaleHandles[i]->setHidden(false);
        m_pScaleHandles[i]->setPosition(centroid + delta);
    }
}

void UIBlockCreationHandle::hideAllScaleHandles()
{
    for ( int i = 0; i < SCALE_HANDLE_COUNT; i++)
        m_pScaleHandles[i]->setHidden(true);
}
