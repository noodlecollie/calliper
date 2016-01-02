#include "basegrid.h"
#include "pervertexcolorshader.h"
#include "shaderstack.h"
#include "camera.h"
#include "callipermath.h"
#include "resourcemanager.h"
#include "scene.h"
#include "mapdocument.h"

BaseGrid::BaseGrid(SceneObject *parent) : SceneObject(parent)
{
    // Standard Hammer colours
    m_colMajor = QColor(100, 46, 0);
    m_colMinor = QColor(119,119,119);
    m_colOrigin = QColor(64,119,119);
    m_colStd = QColor(65,65,65);

    setUpGeometry();
}

int BaseGrid::powerTwo() const
{
    return m_iPowerTwo;
}

void BaseGrid::setPowerTwo(int power)
{
    m_iPowerTwo = power;
}

bool BaseGrid::editable() const
{
    return false;
}

void BaseGrid::setUpGeometry()
{
    m_pGeometry->clear();
    m_pGeometry->setShaderOverride(PerVertexColorShader::staticName());
    m_pGeometry->setDrawMode(GL_LINES);

    int baseVertex = m_pGeometry->vertexCount();
    int offset = 0;

    // Origin - through (0,0)
    m_pGeometry->appendVertex(QVector3D(-1,0,0), m_colOrigin);
    m_pGeometry->appendVertex(QVector3D(1,0,0), m_colOrigin);
    m_pGeometry->appendVertex(QVector3D(0,-1,0), m_colOrigin);
    m_pGeometry->appendVertex(QVector3D(0,1,0), m_colOrigin);
    m_pGeometry->appendIndex(baseVertex+offset++);
    m_pGeometry->appendIndex(baseVertex+offset++);
    m_pGeometry->appendIndex(baseVertex+offset++);
    m_pGeometry->appendIndex(baseVertex+offset++);
    m_DrawOffsets.append(QPair<int,int>(baseVertex, offset));

    baseVertex = m_pGeometry->vertexCount();
    offset = 0;

    // Major lines - every 1024 units
    m_pGeometry->appendVertex(QVector3D(-1,0,0), m_colMajor);
    m_pGeometry->appendVertex(QVector3D(1,0,0), m_colMajor);
    m_pGeometry->appendVertex(QVector3D(0,-1,0), m_colMajor);
    m_pGeometry->appendVertex(QVector3D(0,1,0), m_colMajor);
    m_pGeometry->appendIndex(baseVertex+offset++);
    m_pGeometry->appendIndex(baseVertex+offset++);
    m_pGeometry->appendIndex(baseVertex+offset++);
    m_pGeometry->appendIndex(baseVertex+offset++);
    m_DrawOffsets.append(QPair<int,int>(baseVertex, offset));

    baseVertex = m_pGeometry->vertexCount();

    // Minor lines - every 512 units to every 64 units, depending on density
    // Here we do all of the X lines before the Y lines.
    // First two vertices at Y=1 define the 512 unit gridlines;
    // the next two at Y=0.5 define the 256 unit gridlines;
    // and so on until Y=0.125 for 64 unit gridlines.
    // Depending on the grid density, a subset of this collection
    // of lines may be drawn.

    // X
    for (int i = 1; i <= 8; i *= 2)
    {
        m_pGeometry->appendVertex(QVector3D(-1, 1.0f/(float)i, 0), m_colMinor);
        m_pGeometry->appendVertex(QVector3D(1, 1.0f/(float)i, 0), m_colMinor);
        m_pGeometry->appendIndex(baseVertex+offset++);
        m_pGeometry->appendIndex(baseVertex+offset++);
    }

    // Y
    for (int i = 1; i <= 8; i *= 2)
    {
        m_pGeometry->appendVertex(QVector3D(1.0f/(float)i, -1, 0), m_colMinor);
        m_pGeometry->appendVertex(QVector3D(1.0f/(float)i, 1, 0), m_colMinor);
        m_pGeometry->appendIndex(baseVertex+offset++);
        m_pGeometry->appendIndex(baseVertex+offset++);
    }

    m_DrawOffsets.append(QPair<int,int>(baseVertex, offset));

    baseVertex = m_pGeometry->vertexCount();
    offset = 0;

    // Standard lines - every 32 units to every 1 unit.
    // Same rules as above apply.

    // X
    for ( int i = 1; i <= 32; i *= 2 )
    {
        m_pGeometry->appendVertex(QVector3D(-1, 1.0f/(float)i, 0), m_colMinor);
        m_pGeometry->appendVertex(QVector3D(1, 1.0f/(float)i, 0), m_colMinor);
        m_pGeometry->appendIndex(baseVertex+offset++);
        m_pGeometry->appendIndex(baseVertex+offset++);
    }

    // Y
    for ( int i = 1; i <= 32; i *= 2 )
    {
        m_pGeometry->appendVertex(QVector3D(1.0f/(float)i, -1, 0), m_colMinor);
        m_pGeometry->appendVertex(QVector3D(1.0f/(float)i, 1, 0), m_colMinor);
        m_pGeometry->appendIndex(baseVertex+offset++);
        m_pGeometry->appendIndex(baseVertex+offset++);
    }

    m_DrawOffsets.append(QPair<int,int>(baseVertex, offset));
}

void BaseGrid::draw(ShaderStack *stack)
{
    // Get the current camera.
    const Camera* camera = stack->camera();

    // Determine the bounds of the camera viewing volume.
    BoundingBox bbox = camera->lens().localViewVolumeBounds()
            .transformed(camera->rootToLocal().inverted() * Math::openGLToHammer());

    // If the Z=0 plane is not within this volume, don't draw anything.
    if ( bbox.min().z() > 0 || bbox.max().z() < 0 )
        return;

    MapDocument* doc = scene()->document();
    Q_ASSERT(doc);

    stack->shaderPush(resourceManager()->shader(m_pGeometry->shaderOverride()));
    stack->fogColorPush();
    stack->fogBeginPush();
    stack->fogEndPush();

    // Set up the fog to fade the grid out.
    stack->fogColorSetTop(doc->backgroundColor());
    stack->fogEndSetTop(stack->camera()->lens().farPlane());
    stack->fogBeginSetTop(stack->fogEndTop() -
                          (0.5f * (stack->camera()->lens().farPlane() - stack->camera()->lens().nearPlane())));

    m_pGeometry->upload();
    m_pGeometry->bindVertices(true);
    m_pGeometry->bindIndices(true);
    m_pGeometry->applyDataFormat(stack->shaderTop());

    drawOriginLines(stack, bbox);
    drawMajorLines(stack, bbox);

    stack->fogEndPop();
    stack->fogBeginPop();
    stack->fogColorPop();
    stack->shaderPop();
}

void BaseGrid::drawOriginLines(ShaderStack *stack, const BoundingBox &bbox)
{
    // Draw x and y separately.
    // Taking x as an example, we want to translate the line on x
    // so that it's level with the centre of the view volume, and then
    // scale it up so that it is wide enough to be drawn right across the volume.

    QVector3D centroid = bbox.centroid();
    QVector3D extent = bbox.max() - bbox.min();
    QPair<int,int> offsets = m_DrawOffsets.at(Origin);

    if ( bbox.min().y() <= 0 && bbox.max().y() >= 0 )
    {
        // X
        stack->modelToWorldSetToIdentity();
        stack->modelToWorldPostMultiply(Math::matrixTranslate(QVector3D(centroid.x(),0,0))
                                        * Math::matrixScale(QVector3D(extent.x()/2.0f,1,1)));
        m_pGeometry->draw(offsets.first * sizeof(unsigned int), 2);
    }

    if ( bbox.min().x() <= 0 && bbox.max().x() >= 0 )
    {
        // Y
        stack->modelToWorldSetToIdentity();
        stack->modelToWorldPostMultiply(Math::matrixTranslate(QVector3D(0,centroid.y(),0))
                                        * Math::matrixScale(QVector3D(1,extent.y()/2.0f,1)));
        m_pGeometry->draw((offsets.first+2) * sizeof(unsigned int), 2);
    }
}

void BaseGrid::drawMajorLines(ShaderStack *stack, const BoundingBox &bbox)
{
    // Draw X and Y separately.
    // Taking X as an example, firstly we want to scale the line up to cover
    // the extent of the bounding box, and translate it to be level with the centroid.
    // After this, we want to draw it as many times as there are non-origin 1024-unit
    // gridlines within the given bounding box.

    QVector3D min = bbox.min(), max = bbox.max();
    QVector3D centroid = bbox.centroid();
    QVector3D extent = bbox.max() - bbox.min();
    QPair<int,int> offsets = m_DrawOffsets.at(Major);

    // X
    stack->modelToWorldSetToIdentity();
    stack->modelToWorldPostMultiply(Math::matrixTranslate(QVector3D(centroid.x(),0,0))
                                    * Math::matrixScale(QVector3D(extent.x()/2.0f,1,1)));

    stack->setAutoUpdate(false);
    for ( qint64 i = Math::nextMultiple(min.y(), 1024); (float)i <= max.y(); i += 1024 )
    {
        // Don't draw on origin lines.
        if ( i == 0 )
            continue;

        stack->modelToWorldPush();
        stack->modelToWorldPreMultiply(Math::matrixTranslate(QVector3D(0,(float)i,0)));
        stack->modelToWorldApply();
        m_pGeometry->draw(offsets.first * sizeof(unsigned int), 2);
        stack->modelToWorldPop();
    }
    stack->setAutoUpdate(true);

    // Y
    stack->modelToWorldSetToIdentity();
    stack->modelToWorldPostMultiply(Math::matrixTranslate(QVector3D(0,centroid.y(),0))
                                    * Math::matrixScale(QVector3D(1,extent.y()/2.0f,1)));

    stack->setAutoUpdate(false);
    for ( qint64 i = Math::nextMultiple(min.x(), 1024); (float)i <= max.x(); i += 1024 )
    {
        // Don't draw on origin lines.
        if ( i == 0 )
            continue;

        stack->modelToWorldPush();
        stack->modelToWorldPreMultiply(Math::matrixTranslate(QVector3D((float)i,0,0)));
        stack->modelToWorldApply();
        m_pGeometry->draw((offsets.first+2) * sizeof(unsigned int), 2);
        stack->modelToWorldPop();
    }
    stack->setAutoUpdate(true);
}
