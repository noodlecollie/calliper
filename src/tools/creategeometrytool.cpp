#include "creategeometrytool.h"
#include "mapdocument.h"
#include "mapscene.h"
#include "blockcreationhandle.h"

CreateGeometryTool::CreateGeometryTool(MapDocument *document) : BaseTool(CreateGeometryTool::staticName(), document)
{
    m_pManipulator = NULL;
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

void CreateGeometryTool::vActivate()
{
    BaseTool::vActivate();

    m_pManipulator = m_pDocument->scene()->createSceneObject<BlockCreationHandle>(m_pDocument->scene()->root());
    m_pManipulator->setObjectName("_geometryCreationHandle");
    m_pManipulator->setHidden(true);
}

void CreateGeometryTool::vDeactivate()
{
    m_pDocument->scene()->destroySceneObject(m_pManipulator);
    m_pManipulator = NULL;

    BaseTool::vDeactivate();
}
