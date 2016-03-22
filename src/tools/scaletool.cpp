#include "scaletool.h"

ScaleTool::ScaleTool(MapDocument *document) : BaseTool(ScaleTool::staticName(), document)
{

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
