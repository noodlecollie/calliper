#include "debugcube.h"
#include "model/factories/geometryfactory.h"
#include "model/scene/scene.h"

namespace Model
{
    DebugCube::DebugCube(const SceneObjectInitParams &initParams, SceneObject* parentObject)
        : SceneObject(initParams, parentObject)
    {
        commonInit();
    }

    DebugCube::DebugCube(const DebugCube* cloneFrom, const SceneObjectInitParams &initParams)
        : SceneObject(cloneFrom, initParams)
    {
        commonInit();

        m_flRadius = cloneFrom->m_flRadius;
        m_bDrawFrame = cloneFrom->m_bDrawFrame;
    }

    DebugCube::~DebugCube()
    {

    }

    void DebugCube::commonInit()
    {
        m_flRadius = 1;
        m_bDrawFrame = false;
    }

    void DebugCube::bakeGeometry(RenderSystem::GeometryBuilder &builder) const
    {
        builder.setMaterialId(1);
        GeometryFactory::cube(builder, m_flRadius, QColor::fromRgb(0xffffffff));

        if ( m_bDrawFrame )
        {
            // TODO: Set material in wireframe cube.
            GeometryFactory::wireframeCube(builder, m_flRadius, QColor::fromRgb(0xffff0000));
        }
    }

    float DebugCube::radius() const
    {
        return m_flRadius;
    }

    void DebugCube::setRadius(float r)
    {
        if ( m_flRadius == r )
            return;

        m_flRadius = r;
        flagNeedsRendererUpdate();
    }

    bool DebugCube::drawFrame() const
    {
        return m_bDrawFrame;
    }

    void DebugCube::setDrawFrame(bool draw)
    {
        if ( draw == m_bDrawFrame )
            return;

        m_bDrawFrame = draw;
        flagNeedsRendererUpdate();
    }
}
