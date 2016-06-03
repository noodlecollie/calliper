#include "blockcreationhandle.h"
#include "geometryfactory.h"
#include "shaders.h"
#include "basescene.h"

#define COL_VALID 0x80ffffff
#define COL_INVALID 0x80ff0000

// Panels are created in XY plane. Only the X and Y bounds of the bounding box are considered.
// Colours array is an array of 8 colours, beginning at +X and continuing
// anticlockwise around the face. They correspond to the edges and corners.
// Width is the width of each of the panels, as a fraction of the shortest dimension.
void appendFaceDragPanels(GeometryData* geom, const BoundingBox dimensions, const QRgb* colours, const QMatrix4x4 transform = QMatrix4x4(), float width = 0.2f)
{
    if ( dimensions.isEmpty() )
        return;

    float shortestDim = qMin(dimensions.span(0), dimensions.span(1));
    float panelWidth = width * shortestDim;
    float panelPadding = (shortestDim - (2.0f * panelWidth)) * 0.1f;

    for ( int i = 0; i < 8; i++ )
    {
        float xmin = 0, xmax = 0, ymin = 0, ymax = 0;

        switch (i)
        {
            case 0:
            {
                xmin = dimensions.max().x() - panelWidth;
                xmax = dimensions.max().x();
                ymin = dimensions.min().y() + panelWidth + panelPadding;
                ymax = dimensions.max().y() - panelWidth - panelPadding;
                break;
            }

            case 1:
            {
                xmin = dimensions.max().x() - panelWidth;
                xmax = dimensions.max().x();
                ymin = dimensions.min().y() + panelWidth;
                ymax = dimensions.max().y();
                break;
            }

            case 2:
            {
                xmin = dimensions.min().x() + panelWidth + panelPadding;
                xmax = dimensions.max().x() - panelWidth - panelPadding;
                ymin = dimensions.max().y() - panelWidth;
                ymax = dimensions.max().y();
                break;
            }

            case 3:
            {
                xmin = dimensions.min().x();
                xmax = dimensions.min().x() + panelWidth;
                ymin = dimensions.max().y() - panelWidth;
                ymax = dimensions.max().y();
                break;
            }

            case 4:
            {
                xmin = dimensions.min().x();
                xmax = dimensions.min().x() + panelWidth;
                ymin = dimensions.min().y() + panelWidth + panelPadding;
                ymax = dimensions.max().y() - panelWidth - panelPadding;
                break;
            }

            case 5:
            {
                xmin = dimensions.min().x();
                xmax = dimensions.min().x() + panelWidth;
                ymin = dimensions.min().y();
                ymax = dimensions.min().y() + panelWidth;
                break;
            }

            case 6:
            {
                xmin = dimensions.min().x() + panelWidth + panelPadding;
                xmax = dimensions.max().x() - panelWidth - panelPadding;
                ymin = dimensions.min().y();
                ymax = dimensions.min().y() + panelWidth;
                break;
            }

            case 7:
            {
                xmin = dimensions.max().x() - panelWidth;
                xmax = dimensions.max().x();
                ymin = dimensions.min().y();
                ymax = dimensions.min().y() + panelWidth;
                break;
            }

            default:
                break;
        }

        // TODO: Continue.
    }
}

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
    m_bDrawFaces = false;
    setDrawFaces(true);
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

    if ( m_bDrawFaces )
    {
        GeometryData* faces = GeometryFactory::cuboidSolidColor(m_Bounds, QColor::fromRgba(m_Bounds.hasZeroVolume() ? COL_INVALID : COL_VALID));
        faces->setShaderOverride(PerVertexColorShader::staticName());
        appendGeometry(faces);
    }

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

bool BlockCreationHandle::drawFaces() const
{
    return m_bDrawFaces;
}

void BlockCreationHandle::setDrawFaces(bool enabled)
{
    if ( enabled == m_bDrawFaces )
        return;

    m_bDrawFaces = enabled;
    if ( m_bDrawFaces )
    {
        setRenderFlags(renderFlags() | Translucent);
    }
    else
    {
        setRenderFlags(renderFlags() & ~Translucent);
    }

    m_bRebuildGeometry = true;
}
