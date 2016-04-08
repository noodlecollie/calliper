#include "scaletool.h"
#include "scalehandle.h"
#include "mapdocument.h"
#include "uiscene.h"

ScaleTool::ScaleTool(MapDocument *document) : BaseTool(ScaleTool::staticName(), document)
{
    m_pHandle = NULL;
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
