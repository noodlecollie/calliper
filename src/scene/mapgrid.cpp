#include "mapgrid.h"
#include "pervertexcolorshader.h"
#include "shaderstack.h"
#include "scenecamera.h"
#include "callipermath.h"
#include "resourcemanager.h"
#include "mapscene.h"
#include "mapdocument.h"
#include <QtMath>

#define POWER2_1024 10
#define POWER2_512  9
#define POWER2_256  8
#define POWER2_128  7
#define POWER2_64   6
#define POWER2_32   5
#define POWER2_16   4
#define POWER2_8    3
#define POWER2_4    2
#define POWER2_2    1
#define POWER2_1    0

#define STDLINE_LOD_DELTA 128.0f

MapGrid::MapGrid(BaseScene *scene, SceneObject *parent) : SceneObject(scene, parent)
{
    Q_ASSERT(m_pScene);
    MapScene* s = m_pScene->mapScene();
    Q_ASSERT(s);
    Q_ASSERT(!s->m_pGrid);
    s->m_pGrid = this;

    // Standard Hammer colours
    m_colMajor = QColor(100, 46, 0);
    m_colMinor = QColor(119,119,119);
    m_colOrigin = QColor(64,119,119);
    m_colStd = QColor(65,65,65);
    m_iPowerTwo = 6;    // 64 units

    setUpGeometry();
}

MapGrid::~MapGrid()
{
}

int MapGrid::gridPower() const
{
    return m_iPowerTwo;
}

void MapGrid::setGridPower(int power)
{
    m_iPowerTwo = power;
    if ( m_iPowerTwo < POWER2_1 )
        m_iPowerTwo = POWER2_1;
    else if ( m_iPowerTwo > POWER2_1024 )
        m_iPowerTwo = POWER2_1024;

    emit gridPowerChanged();
}

void MapGrid::incrementGridPower()
{
    setGridPower(m_iPowerTwo+1);
}

void MapGrid::decrementGridPower()
{
    setGridPower(m_iPowerTwo-1);
}

bool MapGrid::editable() const
{
    return false;
}

bool MapGrid::isBackground() const
{
    return true;
}

void MapGrid::setUpGeometry()
{
    GeometryData* geom = new GeometryData();
    appendGeometry(geom);

    geom->clear();
    geom->setShaderOverride(PerVertexColorShader::staticName());
    geom->setDrawMode(GL_LINES);

    int baseVertex = geom->vertexCount();
    int offset = 0;

    // Origin - through (0,0)
    geom->appendVertex(QVector3D(-1,0,0), QVector3D(), m_colOrigin);
    geom->appendVertex(QVector3D(1,0,0), QVector3D(), m_colOrigin);
    geom->appendVertex(QVector3D(0,-1,0), QVector3D(), m_colOrigin);
    geom->appendVertex(QVector3D(0,1,0), QVector3D(), m_colOrigin);
    geom->appendIndex(baseVertex+offset++);
    geom->appendIndex(baseVertex+offset++);
    geom->appendIndex(baseVertex+offset++);
    geom->appendIndex(baseVertex+offset++);
    m_DrawOffsets.append(QPair<int,int>(baseVertex, offset));

    baseVertex = geom->vertexCount();
    offset = 0;

    // Major lines - every 1024 units
    geom->appendVertex(QVector3D(-1,0,0), QVector3D(), m_colMajor);
    geom->appendVertex(QVector3D(1,0,0), QVector3D(), m_colMajor);
    geom->appendVertex(QVector3D(0,-1,0), QVector3D(), m_colMajor);
    geom->appendVertex(QVector3D(0,1,0), QVector3D(), m_colMajor);
    geom->appendIndex(baseVertex+offset++);
    geom->appendIndex(baseVertex+offset++);
    geom->appendIndex(baseVertex+offset++);
    geom->appendIndex(baseVertex+offset++);
    m_DrawOffsets.append(QPair<int,int>(baseVertex, offset));

    baseVertex = geom->vertexCount();
    offset = 0;

    // Minor lines - every 512 units to every 64 units, depending on density
    // Here we do all of the X lines before the Y lines.
    // First two vertices at Y=1 define the 512 unit gridlines;
    // the next two at Y=0.5 define the 256 unit gridlines;
    // and so on until 64 unit gridlines.
    // There is 1 512-unit line, 1 256-unit line, 2 128-unit lines and 4 64-unit lines.
    // Depending on the grid density, a subset of this collection
    // of lines may be drawn.

    // X
    {
        bool slotsUsed[8] = { false, false, false, false, false, false, false, false };
        for (int i = 0; i < 4; i++)
        {
            int intDelta = 8 >> i;
            int acc = 8;

            while (acc > 0)
            {
                if ( !slotsUsed[acc-1] )
                {
                    float f = (float)acc/8.0f;
                    geom->appendVertex(QVector3D(-1, f, 0), QVector3D(), m_colMinor);
                    geom->appendVertex(QVector3D(1, f, 0), QVector3D(), m_colMinor);
                    geom->appendIndex(baseVertex+offset++);
                    geom->appendIndex(baseVertex+offset++);
                    slotsUsed[acc-1] = true;
                }

                acc -= intDelta;
            }
        }
    }

    // Y
    {
        bool slotsUsed[8] = { false, false, false, false, false, false, false, false };
        for (int i = 0; i < 4; i++)
        {
            int intDelta = 8 >> i;
            int acc = 8;

            while (acc > 0)
            {
                if ( !slotsUsed[acc-1] )
                {
                    float f = (float)acc/8.0f;
                    geom->appendVertex(QVector3D(f, -1, 0), QVector3D(), m_colMinor);
                    geom->appendVertex(QVector3D(f, 1, 0), QVector3D(), m_colMinor);
                    geom->appendIndex(baseVertex+offset++);
                    geom->appendIndex(baseVertex+offset++);
                    slotsUsed[acc-1] = true;
                }

                acc -= intDelta;
            }
        }
    }

    m_DrawOffsets.append(QPair<int,int>(baseVertex, offset));

    baseVertex = geom->vertexCount();
    offset = 0;

    // Standard lines - every 32 units to every 1 unit.
    // Same rules as above apply.

    // X
    {
        bool slotsUsed[32];
        for (int i = 0; i < 32; i++)
            slotsUsed[i] = false;

        for ( int i = 0; i < 6; i++ )
        {
            int intDelta = 32 >> i;
            int acc = 32;

            while (acc > 0)
            {
                if ( !slotsUsed[acc-1] )
                {
                    float f = (float)acc/32.0f;
                    geom->appendVertex(QVector3D(-1, f, 0), QVector3D(), m_colStd);
                    geom->appendVertex(QVector3D(1, f, 0), QVector3D(), m_colStd);
                    geom->appendIndex(baseVertex+offset++);
                    geom->appendIndex(baseVertex+offset++);
                    slotsUsed[acc-1] = true;
                }

                acc -= intDelta;
            }
        }
    }

    // Y
    {
        bool slotsUsed[32];
        for (int i = 0; i < 32; i++)
            slotsUsed[i] = false;

        for ( int i = 0; i < 6; i++ )
        {
            int intDelta = 32 >> i;
            int acc = 32;

            while (acc > 0)
            {
                if ( !slotsUsed[acc-1] )
                {
                    float f = (float)acc/32.0f;
                    geom->appendVertex(QVector3D(f, -1, 0), QVector3D(), m_colStd);
                    geom->appendVertex(QVector3D(f, 1, 0), QVector3D(), m_colStd);
                    geom->appendIndex(baseVertex+offset++);
                    geom->appendIndex(baseVertex+offset++);
                    slotsUsed[acc-1] = true;
                }

                acc -= intDelta;
            }
        }
    }

    m_DrawOffsets.append(QPair<int,int>(baseVertex, offset));
}

void MapGrid::draw(ShaderStack *stack)
{
    // Get the current camera and lens.
    const HierarchicalObject* hCamera = stack->cameraParams().hierarchicalObject();
    Q_ASSERT(hCamera);
    const CameraLens* lens = stack->cameraParams().lens();
    Q_ASSERT(lens);

    // Determine the bounds of the camera viewing volume.
    BoundingBox bbox = lens->localViewVolumeBounds()
            .transformed(hCamera->rootToLocal().inverted() * Math::openGLToHammer());

    // If the Z=0 plane is not within this volume, don't draw anything.
    if ( bbox.min().z() > 0 || bbox.max().z() < 0 )
        return;

    MapDocument* doc = scene()->document();
    Q_ASSERT(doc);

    GeometryDataPointer &geom = m_GeometryList[0];

    stack->shaderPush(resourceManager()->shader(geom->shaderOverride()));
    stack->fogColorPush();
    stack->fogBeginPush();
    stack->fogEndPush();

    // Set up the fog to fade the grid out.
    stack->fogColorSetTop(doc->backgroundColor());
    stack->fogEndSetTop(lens->farPlane());
    stack->fogBeginSetTop(stack->fogEndTop() -
                          (0.5f * (lens->farPlane() - lens->nearPlane())));

    geom->upload();
    geom->bindVertices(true);
    geom->bindIndices(true);
    geom->applyDataFormat(stack->shaderTop());

    drawOriginLines(stack, bbox, geom);
    drawMajorLines(stack, bbox, geom);
    drawMinorLines(stack, bbox, geom);
    drawStandardLines(stack, bbox, geom);

    stack->fogEndPop();
    stack->fogBeginPop();
    stack->fogColorPop();
    stack->shaderPop();
}

void MapGrid::drawOriginLines(ShaderStack *stack, const BoundingBox &bbox, GeometryDataPointer &geom)
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
        geom->draw(offsets.first * sizeof(unsigned int), 2);
    }

    if ( bbox.min().x() <= 0 && bbox.max().x() >= 0 )
    {
        // Y
        stack->modelToWorldSetToIdentity();
        stack->modelToWorldPostMultiply(Math::matrixTranslate(QVector3D(0,centroid.y(),0))
                                        * Math::matrixScale(QVector3D(1,extent.y()/2.0f,1)));
        geom->draw((offsets.first+2) * sizeof(unsigned int), 2);
    }
}

void MapGrid::drawMajorLines(ShaderStack *stack, const BoundingBox &bbox, GeometryDataPointer &geom)
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
        geom->draw(offsets.first * sizeof(unsigned int), 2);
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
        geom->draw((offsets.first+2) * sizeof(unsigned int), 2);
        stack->modelToWorldPop();
    }
    stack->setAutoUpdate(true);
    stack->modelToWorldApply();
}

void MapGrid::drawMinorLines(ShaderStack *stack, const BoundingBox &bbox, GeometryDataPointer &geom)
{
    // Minor lines begin at 1 (for a 512 unit gridline) and end at
    // 0.125 (for a 64 unit gridline).
    // Taking X as an example, firstly we want to scale the lines up by 512
    // on Y and enough to fill the bbox extent on x.
    // Then, for each 512-unit section, we want to draw as many lines as our
    // grid density will allow.

    // Do a cheap Z test to see whether we should limit the density
    // of the grid for performance purposes.
    const HierarchicalObject* hCamera = stack->cameraParams().hierarchicalObject();
    Q_ASSERT(hCamera);
    int power = limitGridPower((hCamera->rootToLocal().inverted() * QVector4D(0,0,0,1)).z());

    // Don't draw if we don't have a high enough grid density.
    if ( power >= POWER2_1024 )
        return;

    QVector3D min = bbox.min(), max = bbox.max();
    QVector3D centroid = bbox.centroid();
    QVector3D extent = bbox.max() - bbox.min();
    QPair<int,int> offsets = m_DrawOffsets.at(Minor);

    int verts = 0;
    switch (power)
    {
        case POWER2_512:
            verts = 2;
            break;

        case POWER2_256:
            verts = 4;
            break;

        case POWER2_128:
            verts = 8;
            break;

        default:
            verts = 16;
            break;
    }

    int count = qMin(offsets.second, verts);

    // X
    stack->modelToWorldSetToIdentity();
    stack->modelToWorldPostMultiply(Math::matrixTranslate(QVector3D(centroid.x(),0,0))
                                    * Math::matrixScale(QVector3D(extent.x()/2.0f,512,1)));
    stack->setAutoUpdate(false);
    for ( qint64 i = Math::previousMultiple(min.y(), 512); i <= max.y(); i += 512 )
    {
        // If we would draw over a previously coloured line, modify how many lines we draw.
        int localCount = count;
        int localOffset = offsets.first;
        if ( (i-512) % 1024 == 0 )
        {
            localCount -= 2;
            localOffset += 2;
        }

        stack->modelToWorldPush();
        stack->modelToWorldPreMultiply(Math::matrixTranslate(QVector3D(0,(float)i,0)));
        stack->modelToWorldApply();
        geom->draw(localOffset * sizeof(unsigned int), localCount);
        stack->modelToWorldPop();
    }
    stack->setAutoUpdate(true);

    // Y
    stack->modelToWorldSetToIdentity();
    stack->modelToWorldPostMultiply(Math::matrixTranslate(QVector3D(0,centroid.y(),0))
                                    * Math::matrixScale(QVector3D(512,extent.y()/2.0f,1)));
    stack->setAutoUpdate(false);
    for ( qint64 i = Math::previousMultiple(min.x(), 512); i <= max.x(); i += 512 )
    {
        // If we would draw over a previously coloured line, modify how many lines we draw.
        int localCount = count;
        int localOffset = offsets.first + 16;
        if ( (i-512) % 1024 == 0 )
        {
            localCount -= 2;
            localOffset += 2;
        }

        stack->modelToWorldPush();
        stack->modelToWorldPreMultiply(Math::matrixTranslate(QVector3D((float)i,0,0)));
        stack->modelToWorldApply();
        geom->draw(localOffset * sizeof(unsigned int), localCount);
        stack->modelToWorldPop();
    }
    stack->setAutoUpdate(true);

    stack->modelToWorldApply();
}

void MapGrid::drawStandardLines(ShaderStack *stack, const BoundingBox &bbox, GeometryDataPointer &geom)
{
    // Standard lines begin at 1 (for a 32 unit gridline) and end at
    // 1/32 (for a 1 unit gridline).
    // Taking X as an example, firstly we want to scale the lines up by 32
    // on Y and enough to fill the bbox extent on x.
    // Then, for each 32-unit section, we want to draw as many lines as our
    // grid density will allow.

    // Do a cheap Z test to see whether we should limit the density
    // of the grid for performance purposes.
    const HierarchicalObject* hCamera = stack->cameraParams().hierarchicalObject();
    QMatrix4x4 camToWorld = hCamera->rootToLocal().inverted();
    QVector3D camWorldPos = (camToWorld * QVector4D(0,0,0,1)).toVector3D();
    Q_ASSERT(hCamera);
    int power = limitGridPower(camWorldPos.z());

    // Don't draw if we don't have a high enough grid density.
    if ( power >= POWER2_64 )
        return;

    QVector3D min = bbox.min(), max = bbox.max();
    QVector3D centroid = bbox.centroid();
    QVector3D extent = bbox.max() - bbox.min();
    QPair<int,int> offsets = m_DrawOffsets.at(Std);

    // X
    stack->modelToWorldSetToIdentity();
    stack->modelToWorldPostMultiply(Math::matrixTranslate(QVector3D(centroid.x(),0,0))
                                    * Math::matrixScale(QVector3D(extent.x()/2.0f,32,1)));
    stack->setAutoUpdate(false);
    for ( qint64 i = Math::previousMultiple(min.y(), 32); i <= max.y(); i += 32 )
    {
        float dist = qAbs(camWorldPos.y() - (float)(i+16));
        int m = qFloor(dist/STDLINE_LOD_DELTA);
        int localPower = power + m;

        // If we would draw over a previously coloured line, modify how many lines we draw.
        int localCount = stdLineVertCount(localPower);
        int localOffset = offsets.first;
        if ( (i-32) % 64 == 0 )
        {
            localCount -= 2;
            localOffset += 2;
        }

        if ( localCount <= 0 )
        {
            continue;
        }

        stack->modelToWorldPush();
        stack->modelToWorldPreMultiply(Math::matrixTranslate(QVector3D(0,(float)i,0)));
        stack->modelToWorldApply();
        geom->draw(localOffset * sizeof(unsigned int), localCount);
        stack->modelToWorldPop();
    }
    stack->setAutoUpdate(true);

    // Y
    stack->modelToWorldSetToIdentity();
    stack->modelToWorldPostMultiply(Math::matrixTranslate(QVector3D(0,centroid.y(),0))
                                    * Math::matrixScale(QVector3D(32,extent.y()/2.0f,1)));
    stack->setAutoUpdate(false);
    for ( qint64 i = Math::previousMultiple(min.x(), 32); i <= max.x(); i += 32 )
    {
        float dist = qAbs(camWorldPos.x() - (float)(i+16));
        int m = qFloor(dist/STDLINE_LOD_DELTA);
        int localPower = power + m;

        // If we would draw over a previously coloured line, modify how many lines we draw.
        int localCount = stdLineVertCount(localPower);
        int localOffset = offsets.first + 64;
        if ( (i-32) % 64 == 0 )
        {
            localCount -= 2;
            localOffset += 2;
        }

        if ( localCount <= 0 )
        {
            continue;
        }

        stack->modelToWorldPush();
        stack->modelToWorldPreMultiply(Math::matrixTranslate(QVector3D((float)i,0,0)));
        stack->modelToWorldApply();
        geom->draw(localOffset * sizeof(unsigned int), localCount);
        stack->modelToWorldPop();
    }
    stack->setAutoUpdate(true);

    stack->modelToWorldApply();
}

int MapGrid::limitGridPower(float camWorldZ) const
{
    // Return a LOD'ed grid power depending on how many multiples of 64 we are away on Z.
    int lod = Math::previousMultiple(qAbs(camWorldZ), 64)/64;
    return qMax(m_iPowerTwo, qMin(POWER2_1 + lod, POWER2_128));
}

int MapGrid::stdLineVertCount(int power)
{
    if ( power > 5 || power < 0 )
        return 0;

    return 1 << (6 - power);
}

unsigned int MapGrid::gridMultiple() const
{
    return 1 << m_iPowerTwo;
}
