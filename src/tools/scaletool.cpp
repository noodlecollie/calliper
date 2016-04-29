#include "scaletool.h"
#include "scalehandle.h"
#include "mapdocument.h"
#include "uiscene.h"
#include "viewport.h"
#include "application.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include "mapscene.h"

ScaleTool::ScaleTool(MapDocument *document) : BaseTool(ScaleTool::staticName(), document)
{
    m_pHandle = NULL;
    m_bInMove = false;
}

ScaleTool::~ScaleTool()
{
    if ( m_bActive )
    {
        deactivate();
    }
}

QString ScaleTool::staticName()
{
    return QString("ScaleTool");
}

void ScaleTool::vActivate()
{
    m_pHandle = new ScaleHandle(m_pDocument->uiScene()->root());
    m_pHandle->setObjectName("_scaleHandle");
    m_vecScale = QVector3D(1,1,1);
    updateTableFromSet();
    updateHandleState();
    m_bInMove = false;
}

void ScaleTool::vDeactivate()
{
    if ( m_bInMove )
        endMove();

    clearTableManipulators();
    delete m_pHandle;
    m_pHandle = NULL;
    BaseTool::vDeactivate();
}

void ScaleTool::vKeyPress(QKeyEvent *e)
{
    if ( m_bInMove )
        return;

    BaseTool::vKeyPress(e);
}

void ScaleTool::vKeyRelease(QKeyEvent *e)
{
    if ( m_bInMove )
        return;

    BaseTool::vKeyRelease(e);
}

void ScaleTool::vSelectedSetChanged()
{
    if ( m_bInMove )
        endMove();

    updateTableFromSet();
    updateHandleState();
    BaseTool::vSelectedSetChanged();
}


void ScaleTool::vMousePress(QMouseEvent *e)
{
    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v || !v->camera() )
        return;

    QRgb col = 0xffffffff;
    SceneObject* obj = v->pickObjectFromDepthBuffer(MapDocument::UISceneFlag | MapDocument::MapSceneFlag, e->pos(), &col);

    if ( !obj || obj->scene() == m_pDocument->scene() )
    {
        addToSelectedSet(obj, !m_flKBModifiers.testFlag(Qt::ControlModifier));
        return;
    }

    if ( obj != m_pHandle )
    {
        m_pDocument->selectedSetClear();
        return;
    }
}

void ScaleTool::vMouseMove(QMouseEvent *e)
{
    if ( !m_bInMove )
    {
        BaseTool::vMouseMove(e);
        return;
    }
}

void ScaleTool::vMouseRelease(QMouseEvent *e)
{
    if ( !m_bInMove )
    {
        BaseTool::vMouseRelease(e);
        return;
    }

    endMove();
}

void ScaleTool::endMove()
{
    m_bInMove = false;
}

void ScaleTool::updateTableFromSet()
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

void ScaleTool::updateTableManipulators()
{
    for ( ManipTable::iterator it = m_ManipTable.begin(); it != m_ManipTable.end(); ++it )
    {
        SceneObject* obj = it.key();
        SceneObjectManipulator& m = it.value();

        // If the object already has ancestors in the table, don't set its position - it will inherit.
        if ( isAncestorInManipulatorTable(obj) )
            continue;

        m.setScale(m_vecScale);
    }
}

void ScaleTool::commitTableManipulators()
{
    for ( ManipTable::iterator it = m_ManipTable.begin(); it != m_ManipTable.end(); ++it )
    {
        SceneObject* obj = it.key();
        SceneObjectManipulator& m = it.value();

        // If the object already has ancestors in the table, don't do anything - it will inherit.
        if ( isAncestorInManipulatorTable(obj) )
            continue;

        qDebug() << "Applying" << obj << "manipulator scale:" << m.scale();
        m.apply();
    }
}

bool ScaleTool::isAncestorInManipulatorTable(const SceneObject *obj) const
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

void ScaleTool::clearTableManipulators()
{
    m_ManipTable.clear();
}

void ScaleTool::updateHandleState()
{
    Q_ASSERT(m_pHandle);
    const QSet<SceneObject*> &sset = m_pDocument->selectedSet();
    m_pHandle->setPosition(sset.count() == 1 ? (*sset.begin())->localToRoot(QVector3D(0,0,0)) : m_pDocument->selectedSetCentroid());
    m_pHandle->setHidden(sset.count() < 1);
}
