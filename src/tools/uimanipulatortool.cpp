#include "uimanipulatortool.h"
#include "viewport.h"
#include "uimanipulator.h"
#include "application.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include "mapscene.h"
#include "scenecamera.h"
#include "uiscene.h"

UIManipulatorTool::UIManipulatorTool(const QString &name, MapDocument *document) :
    BaseTool(name, document)
{
    m_pManipulator = NULL;
    m_bInMove = false;
    m_flHandleCamDist = 0.0f;
    m_iAxisFlags = Math::NoAxisFlag;
    m_bHoveringOnManipulator = false;
}

UIManipulatorTool::~UIManipulatorTool()
{
    if ( m_bActive )
    {
        deactivate();
    }
}

void UIManipulatorTool::vActivate()
{
    BaseTool::vActivate();
    m_pManipulator = constructManipulator();
    updateTableFromSet();
    updateManipulator();
    m_bInMove = false;
}

void UIManipulatorTool::vDeactivate()
{
    if ( m_bInMove )
        endMove();

    clearTableManipulators();
    m_pDocument->uiScene()->destroySceneObject(m_pManipulator);
    BaseTool::vDeactivate();
}

void UIManipulatorTool::vKeyPress(QKeyEvent *e)
{
    if ( m_bInMove )
        return;

    switch (e->key())
    {
        case Qt::Key_Escape:
        {
            m_pDocument->selectedSetClear();
            break;
        }

        default:
            BaseTool::vKeyPress(e);
            break;
    }
}

void UIManipulatorTool::vKeyRelease(QKeyEvent *e)
{
    if ( m_bInMove )
        return;

    BaseTool::vKeyRelease(e);
}

void UIManipulatorTool::vMousePress(QMouseEvent *e)
{
    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v || !v->camera() )
        return;

    QRgb col = 0xffffffff;
    SceneObject* obj = v->pickObjectFromDepthBuffer(BaseScene::UISceneFlag | BaseScene::MapSceneFlag, e->pos(), SceneObject::AllObjectsMask, &col);

    if ( !obj || obj->scene() == m_pDocument->scene() )
    {
        addToSelectedSet(obj, !m_flKBModifiers.testFlag(Qt::ControlModifier));
        return;
    }

    if ( obj != m_pManipulator )
    {
        m_pDocument->selectedSetClear();
        return;
    }

    // Cache the following:

    m_ManipulatorOriginalOrientation = m_pManipulator->worldSpatialSnapshot();
    m_Transformation = SpatialSnapshot();

    // Original mouse position in the viewport.
    m_BeginDragPos = e->pos();

    // Distance between camera and handle - to maintain when dragging relative to camera.
    m_flHandleCamDist = (m_ManipulatorOriginalOrientation.translation - v->camera()->position()).length();

    // The flags for the axes we're constrained to.
    m_iAxisFlags = UIManipulator::axisFlagsFromPickColor(col);

    // The axes themselves
    m_MovementAxes = UIManipulator::manipulationAxes(m_iAxisFlags);

    m_bInMove = true;
}

void UIManipulatorTool::vMouseMove(QMouseEvent *e)
{
    if ( !m_bInMove )
    {
        BaseTool::vMouseMove(e);
        return;
    }

    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v || !v->camera() )
    {
        BaseTool::vMouseMove(e);
        return;
    }

    updateManipulatorFromMouseMove(e);
}

void UIManipulatorTool::vMouseMoveHover(QMouseEvent *e)
{
    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v || !v->camera() )
    {
        BaseTool::vMouseMoveHover(e);
        return;
    }

    if ( m_bMouseLookEnabled )
    {
        if ( m_bHoveringOnManipulator )
            endManipulatorHover();
        m_bHoveringOnManipulator = false;

        BaseTool::vMouseMoveHover(e);
        return;
    }

    updateManipulatorHoverState(v, e->pos());
}

bool UIManipulatorTool::isMouseOnManipulator(Viewport *v, const QPoint &pos, QRgb *pickColour) const
{
    SceneObject* obj = v->pickObjectFromDepthBuffer(BaseScene::UISceneFlag, pos, SceneObject::AllObjectsMask, pickColour);
    return obj && obj == m_pManipulator;
}

void UIManipulatorTool::vEnter(QEnterEvent *)
{
}

void UIManipulatorTool::vLeave(QEvent *)
{
}

void UIManipulatorTool::vMouseRelease(QMouseEvent *e)
{
    if ( !m_bInMove )
    {
        BaseTool::vMouseRelease(e);
    }

    updateManipulatorForMouseRelease(e);
    endMove();
}

void UIManipulatorTool::vSelectedSetChanged()
{
    if ( m_bInMove )
        endMove();

    updateTableFromSet();
    updateManipulator();
    BaseTool::vSelectedSetChanged();
}

void UIManipulatorTool::updateManipulatorHoverState(Viewport *v, const QPoint &pos)
{
    QRgb col = 0xffffffff;
    if ( isMouseOnManipulator(v, pos, &col) )
    {
        if ( !m_bHoveringOnManipulator )
            startManipulatorHover(col);
        else
            updateManipulatorHover(col);

        m_bHoveringOnManipulator = true;
    }
    else
    {
        if ( m_bHoveringOnManipulator )
            endManipulatorHover();
        m_bHoveringOnManipulator = false;
    }
}

void UIManipulatorTool::endMove()
{
    commitTableManipulators();
    m_bInMove = false;

    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v || !v->camera() )
        return;

    updateManipulatorHoverState(v, v->mapFromGlobal(QCursor::pos()));
}

bool UIManipulatorTool::isAncestorInManipulatorTable(const SceneObject *obj) const
{
    SceneObject* p = obj->parentObject();
    while (p)
    {
        if ( m_ManipTable.contains(p) )
            return true;

        p = p->parentObject();
    }

    return false;
}

void UIManipulatorTool::updateTableFromSet()
{
    typedef QSet<SceneObject*> SOSet;
    const SOSet &set = m_pDocument->selectedSet();

    if ( set.count() < 1 )
    {
        clearTableManipulators();
        return;
    }

    // Remove our manipulators that are no longer present in the set.
    QList<SceneObject*> toRemove;
    for ( ManipTable::const_iterator it = m_ManipTable.constBegin(); it != m_ManipTable.constEnd(); ++it )
    {
        if ( !set.contains(it.key()) )
            toRemove.append(it.key());
    }

    foreach ( SceneObject* o, toRemove )
    {
        m_ManipTable.remove(o);
    }

    // Add new items that are not present in our table.
    for ( SOSet::const_iterator it = set.constBegin(); it != set.constEnd(); ++it )
    {
        if ( !m_ManipTable.contains(*it) )
        {
            m_ManipTable.insert((*it), SceneObjectManipulator(*it));
        }
    }
}

void UIManipulatorTool::updateTableManipulators()
{
    for ( ManipTable::iterator it = m_ManipTable.begin(); it != m_ManipTable.end(); ++it )
    {
        SceneObject* obj = it.key();
        SceneObjectManipulator& m = it.value();

        // If the object already has ancestors in the table, don't set its position - it will inherit.
        if ( isAncestorInManipulatorTable(obj) )
            continue;

        updateSceneObjectManipulator(obj, m);
    }
}

void UIManipulatorTool::commitTableManipulators()
{
    for ( ManipTable::iterator it = m_ManipTable.begin(); it != m_ManipTable.end(); ++it )
    {
        SceneObject* obj = it.key();
        SceneObjectManipulator& m = it.value();

        // If the object already has ancestors in the table, don't do anything - it will inherit.
        if ( isAncestorInManipulatorTable(obj) )
            continue;

        commitSceneObjectManipulator(obj, m);
    }
}

void UIManipulatorTool::clearTableManipulators()
{
    m_ManipTable.clear();
}
