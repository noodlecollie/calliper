#include "basetool.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

BaseTool::BaseTool(const QString &name, MapDocument *document) : QObject(NULL)
{
    setObjectName(name);
    m_bActive = false;
    m_pDocument = document;
    Q_ASSERT(m_pDocument);
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
