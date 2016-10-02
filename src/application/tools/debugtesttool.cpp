#include "debugtesttool.h"
#include <QKeyEvent>
#include "mapdocument.h"
#include "sceneobjectmanipulator.h"
#include "application.h"
#include "mainwindow.h"
#include "viewport.h"
#include "uiscene.h"
#include "translationhandle.h"
#include "geometryfactory.h"
#include "scenecamera.h"
#include "mapscene.h"
#include "mapgrid.h"
#include "callipermath.h"
#include "ray3d.h"
#include "shaders.h"

DebugTestTool::DebugTestTool(MapDocument *document) : BaseTool(DebugTestTool::staticName(), document)
{
    m_pRayVisuals = nullptr;
}

DebugTestTool::~DebugTestTool()
{
    if ( m_bActive )
    {
        deactivate();
    }
}

QString DebugTestTool::staticName()
{
    return QString("DebugTestTool");
}

void DebugTestTool::vKeyPress(QKeyEvent *e)
{
    BaseTool::vKeyPress(e);
}

void DebugTestTool::vSelectedSetChanged()
{
    BaseTool::vSelectedSetChanged();
}

void DebugTestTool::vDeactivate()
{
    BaseTool::vDeactivate();

    m_pDocument->scene()->destroySceneObject(m_pRayVisuals);
    m_pRayVisuals = nullptr;
}

void DebugTestTool::vActivate()
{
    BaseTool::vActivate();

    m_pRayVisuals = m_pDocument->scene()->createSceneObject<SceneObject>(m_pDocument->scene()->generalObjectsNode());
    m_pRayVisuals->appendGeometry(new GeometryData());
    m_pRayVisuals->appendGeometry(new GeometryData());
    m_pRayVisuals->appendGeometry(new GeometryData());
    m_iLastRay = 0;
}

void DebugTestTool::vMousePress(QMouseEvent *e)
{
    //BaseTool::vMousePress(e);

    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v || !v->camera() )
        return;

    SceneCamera* cam = v->camera();
    QVector3D rayDir = cam->frustumDirection(e->pos(), v->size());
    Ray3D ray(cam->position() + (cam->lens()->nearPlane() * rayDir), rayDir);

    RayTraceContact contact;
    SceneObject* hitObject = m_pDocument->scene()->generalObjectsNode()->computeRayCastRecursive(ray, contact);
    if ( hitObject )
    {
        qDebug() << "Ray" << ray << "hit object" << hitObject->objectName() << "at distance" << contact.rayParameter
                 << "to receive colour" << contact.color << "\n";
        buildRayVisuals(ray, contact.rayParameter);
    }
    else
    {
        qDebug() << "Ray" << ray << "did not hit any object.";
        m_pRayVisuals->geometryAt(m_iLastRay)->clear();
    }

    m_iLastRay = (m_iLastRay + 1)%3;
}

void DebugTestTool::buildRayVisuals(const Ray3D &ray, float intersection)
{
    Q_ASSERT(m_pRayVisuals);

    GeometryDataPointer geom = m_pRayVisuals->geometryAt(m_iLastRay);
    geom->setDrawMode(GL_LINES);
    geom->setShaderOverride(PerVertexColorShader::staticName());

    geom->clear();
    geom->appendVertex(ray.origin(), QVector3D(), QColor::fromRgb(0xffff0000));
    geom->appendVertex(ray.parameterise(intersection), QVector3D(), QColor::fromRgb(0xff00ff00));
    geom->appendIndex(0, 1);
}

void DebugTestTool::vMouseMove(QMouseEvent *e)
{
    BaseTool::vMouseMove(e);
}

void DebugTestTool::vMouseRelease(QMouseEvent *e)
{
    BaseTool::vMouseRelease(e);
}
