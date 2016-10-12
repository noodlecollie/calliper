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

    void DebugCube::bakeGeometry(NS_RENDERER::GeometryBuilder &builder) const
    {
        // BAD: Fix shader and texture so they're not hardcoded!
        quint16 oldShader = builder.shaderId();
        builder.setShaderId(1);
        GeometryFactory::cube(builder, m_flRadius, QColor::fromRgb(0xffffffff));

        if ( m_bDrawFrame )
        {
            builder.setShaderId(0);
            GeometryFactory::wireframeCube(builder, m_flRadius, QColor::fromRgb(0xffff0000));
        }

        builder.setShaderId(oldShader);
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
