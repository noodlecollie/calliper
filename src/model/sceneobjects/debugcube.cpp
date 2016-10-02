#include "debugcube.h"
#include "factories/geometryfactory.h"

namespace NS_MODEL
{
    DebugCube::DebugCube(Scene* parentScene, SceneObject* parentObject)
        : SceneObject(parentScene, parentObject)
    {
        commonInit();
    }

    DebugCube::DebugCube(const DebugCube *cloneFrom)
        : SceneObject(cloneFrom)
    {
        m_flRadius = cloneFrom->m_flRadius;
    }

    DebugCube::~DebugCube()
    {

    }

    void DebugCube::commonInit()
    {
        m_flRadius = 1;
    }

    void DebugCube::bakeGeometry(NS_RENDERER::GeometryBuilder &builder) const
    {
        // TODO: Set shader and texture.
        GeometryFactory::cube(builder, m_flRadius, QColor::fromRgb(0xffffffff));
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
}
