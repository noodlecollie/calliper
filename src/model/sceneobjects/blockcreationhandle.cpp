#include "blockcreationhandle.h"
#include "geometryfactory.h"
#include "shaders.h"
#include "basescene.h"

#define COL_VALID 0x80ffffff
#define COL_INVALID 0x80ff0000

namespace NS_MODEL
{
    BlockCreationHandle::BlockCreationHandle(BaseScene *scene, SceneObject *parent) : UIManipulator(scene, parent)
    {
        initDefaults();
    }

    BlockCreationHandle::~BlockCreationHandle()
    {

    }

    void BlockCreationHandle::initDefaults()
    {
        m_bRebuildGeometry = true;
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

        GeometryData* faces = GeometryFactory::cuboidSolidColor(m_Bounds, QColor::fromRgba(m_Bounds.hasZeroVolume() ? COL_INVALID : COL_VALID));
        faces->setShaderOverride(PerVertexColorShader::staticName());
        appendGeometry(faces);

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

        // Bypass UIManipulator::draw because we don't want all that fancy rescaling stuff.
        SceneObject::draw(stack);
    }
}
