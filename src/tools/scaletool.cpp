#include "scaletool.h"
#include "scalehandle.h"
#include "mapdocument.h"
#include "uiscene.h"

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
}

void ScaleTool::vDeactivate()
{
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
    BaseTool::vSelectedSetChanged();
}


void ScaleTool::vMousePress(QMouseEvent *e)
{
    BaseTool::vMousePress(e);
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
