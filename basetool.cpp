#include "basetool.h"
#include "mapdocument.h"
#include "scene.h"
#include "basegrid.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include "viewport.h"
#include "application.h"
#include "mainwindow.h"

BaseTool::BaseTool(const QString &name, MapDocument *document) : QObject(NULL)
{
    setObjectName(name);
    m_bActive = false;
    m_pDocument = document;
    Q_ASSERT(m_pDocument);

    m_flKBModifiers = 0;
}

BaseTool::~BaseTool()
{
    Q_ASSERT(!m_bActive);
}

void BaseTool::activate()
{
    if ( m_bActive )
        return;

    vActivate();
    m_bActive = true;
}

void BaseTool::deactivate()
{
    if ( !m_bActive )
        return;

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

MapDocument* BaseTool::document() const
{
    return m_pDocument;
}

void BaseTool::selectedSetChanged()
{
    Q_ASSERT(m_bActive);
    vSelectedSetChanged();
}

void BaseTool::vActivate()
{
}

void BaseTool::vDeactivate()
{
    m_flKBModifiers = 0;
}

void BaseTool::vMousePress(QMouseEvent *e)
{
    Viewport* v = application()->mainWindow()->activeViewport();
    if ( !v )
        return;

    SceneObject* obj = v->pickObjectFromDepthBuffer(e->pos());

    if ( !m_flKBModifiers.testFlag(Qt::ControlModifier) )
    {
        m_pDocument->selectedSetClear();
        if ( obj )
            m_pDocument->selectedSetInsert(obj);
    }
    else
    {
        if ( !obj )
            return;

        if ( m_pDocument->selectedSet().contains(obj) )
        {
            m_pDocument->selectedSetRemove(obj);
        }
        else
        {
            m_pDocument->selectedSetInsert(obj);
        }
    }
}

void BaseTool::vMouseMove(QMouseEvent *)
{
}

void BaseTool::vMouseRelease(QMouseEvent *)
{
}

void BaseTool::vWheel(QWheelEvent *)
{
}

void BaseTool::vKeyPress(QKeyEvent *e)
{
    if ( e->isAutoRepeat() )
        return;

    switch (e->key())
    {
        case Qt::Key_BracketLeft:
        {
            m_pDocument->scene()->grid()->decrementGridPower();
            return;
        }

        case Qt::Key_BracketRight:
        {
            m_pDocument->scene()->grid()->incrementGridPower();
            return;
        }

        case Qt::Key_Control:
        {
            m_flKBModifiers |= Qt::ControlModifier;
            return;
        }

        default:
            return;
    }
}

void BaseTool::vKeyRelease(QKeyEvent *e)
{
    if ( e->isAutoRepeat() )
        return;

    switch (e->key())
    {
        case Qt::Key_Control:
        {
            m_flKBModifiers &= ~Qt::ControlModifier;
            return;
        }
    }
}

void BaseTool::vSelectedSetChanged()
{
}

Qt::KeyboardModifiers BaseTool::keyboardModifiers() const
{
    return m_flKBModifiers;
}

bool BaseTool::keyboardModifierActive(Qt::KeyboardModifier mod) const
{
    return m_flKBModifiers.testFlag(mod);
}
