#include "blockcreationhandle.h"
#include "geometryfactory.h"
#include "shaders.h"
#include "basescene.h"

BlockCreationHandle::BlockCreationHandle(BaseScene *scene, SceneObject *parent) : UIManipulator(scene, parent)
{
    initDefaults();
}

BlockCreationHandle::~BlockCreationHandle()
{
    bool halt = true;
}

void BlockCreationHandle::initDefaults()
{
    m_bRebuildGeometry = true;
}

SceneObject* BlockCreationHandle::clone() const
{
    return m_pScene->cloneSceneObject<BlockCreationHandle>(this);
}

BlockCreationHandle::BlockCreationHandle(const BlockCreationHandle &cloneFrom) : UIManipulator(cloneFrom)
{
    initDefaults();
    m_Bounds = cloneFrom.m_Bounds;
}

BoundingBox BlockCreationHandle::bounds() const
{
    return m_Bounds;
}

void BlockCreationHandle::setBounds(const BoundingBox &bounds)
{
    if ( bounds == m_Bounds )
        return;

    m_Bounds = bounds;
    m_bRebuildGeometry = true;
}

void BlockCreationHandle::rebuildGeometry()
{
    if ( geometryCount() > 0 )
    {
        clearGeometry();
    }

    GeometryData* geom = GeometryFactory::lineCuboid(m_Bounds, QColor::fromRgb(0xffffffff));
    geom->setShaderOverride(PerVertexColorShader::staticName());
    appendGeometry(geom);

    m_bRebuildGeometry = false;
}

void BlockCreationHandle::setBounds(const QVector3D &min, const QVector3D &max)
{
    setBounds(BoundingBox(min,max));
}

void BlockCreationHandle::draw(ShaderStack *stack)
{
    if ( m_bRebuildGeometry )
        rebuildGeometry();

    SceneObject::draw(stack);
}
