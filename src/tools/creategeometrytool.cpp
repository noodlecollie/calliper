#include "creategeometrytool.h"

CreateGeometryTool::CreateGeometryTool(MapDocument *document) : BaseTool(CreateGeometryTool::staticName(), document)
{

}

CreateGeometryTool::~CreateGeometryTool()
{
    if ( m_bActive )
    {
        deactivate();
    }
}

QString CreateGeometryTool::staticName()
{
    return staticMetaObject.className();
}
