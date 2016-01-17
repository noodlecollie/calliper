#include "debugtesttool.h"
#include <QKeyEvent>
#include "mapdocument.h"
#include "sceneobjectmanipulator.h"
#include "application.h"
#include "mainwindow.h"
#include "viewport.h"

DebugTestTool::DebugTestTool(MapDocument *document) : BaseTool(DebugTestTool::staticName(), document)
{

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
    if ( e->isAutoRepeat() )
        return;

    switch (e->key())
    {
        case Qt::Key_Right:
        {
            m_vecTranslation += QVector3D(64,0,0);
            updateTableManipulators();
            break;
        }

        case Qt::Key_Left:
        {
            m_vecTranslation -= QVector3D(64,0,0);
            updateTableManipulators();
            break;
        }

        case Qt::Key_Enter:
        {
            applyTableManipulators();
            break;
        }

        default:
            BaseTool::vKeyPress(e);
            break;
    }
}

void DebugTestTool::vSelectedSetChanged()
{
    updateTableFromSet();
    BaseTool::vSelectedSetChanged();
}

void DebugTestTool::updateTableFromSet()
{
    typedef QSet<SceneObject*> SOSet;
    const SOSet &set = m_pDocument->selectedSet();

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

void DebugTestTool::updateTableManipulators()
{
    for ( ManipTable::iterator it = m_ManipTable.begin(); it != m_ManipTable.end(); ++it )
    {
        it.value().setTranslation(m_vecTranslation);
    }
}

void DebugTestTool::applyTableManipulators()
{
    for ( ManipTable::iterator it = m_ManipTable.begin(); it != m_ManipTable.end(); ++it )
    {
        it.value().apply();
    }
}

void DebugTestTool::vDeactivate()
{
    m_ManipTable.clear();
    m_vecTranslation = QVector3D(0,0,0);
    BaseTool::vDeactivate();
}
