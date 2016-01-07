#include "basetool.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

BaseTool::BaseTool(const QString &name) : QObject(NULL)
{
    setObjectName(name);
    m_bActive = false;
    m_pDocument = NULL;
}

BaseTool::~BaseTool()
{
    Q_ASSERT(!m_bActive);
}

void BaseTool::activate(MapDocument *doc)
{
    Q_ASSERT(!m_bActive);
    m_pDocument = doc;

    vActivate();

    m_bActive = true;
}

void BaseTool::deactivate()
{
    Q_ASSERT(m_bActive);
    m_pDocument = NULL;

    vDeactivate();

    m_bActive = false;
}

void BaseTool::mousePressEvent(QMouseEvent *e)
{
    Q_ASSERT(m_bActive);
    vMousePress(e);
}

void BaseTool::mouseMoveEvent(QMouseEvent *e)
{
    Q_ASSERT(m_bActive);
    vMouseMove(e);
}

void BaseTool::mouseReleaseEvent(QMouseEvent *e)
{
    Q_ASSERT(m_bActive);
    vMouseRelease(e);
}

void BaseTool::wheelEvent(QWheelEvent *e)
{
    Q_ASSERT(m_bActive);
    vWheel(e);
}

void BaseTool::keyPressEvent(QKeyEvent *e)
{
    Q_ASSERT(m_bActive);
    vKeyPress(e);
}

void BaseTool::keyReleaseEvent(QKeyEvent *e)
{
    Q_ASSERT(m_bActive);
    vKeyRelease(e);
}

bool BaseTool::isActive() const
{
    return m_bActive;
}

QString BaseTool::toolName() const
{
    return QString("INVALID_TOOL_NAME");
}

void BaseTool::addSceneObject(SceneObject *obj)
{
    Q_ASSERT(m_bActive);
    m_SceneObjects.insert(obj);
    vSceneObjectAdded(obj);
}

void BaseTool::removeSceneObject(SceneObject *obj)
{
    Q_ASSERT(m_bActive);
    vSceneObjectRemoved(obj);
    m_SceneObjects.remove(obj);
}

void BaseTool::clearSceneObjects()
{
    Q_ASSERT(m_bActive);
    m_SceneObjects.clear();
    vSceneObjectsCleared();
}

bool BaseTool::containsSceneObject(SceneObject *obj) const
{
    return m_SceneObjects.contains(obj);
}
