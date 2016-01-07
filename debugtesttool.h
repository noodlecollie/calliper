#ifndef DEBUGTESTTOOL_H
#define DEBUGTESTTOOL_H

#include "basetool.h"
#include <QHash>
#include "sceneobjectmanipulator.h"

class SceneObject;

class DebugTestTool : public BaseTool
{
public:
    DebugTestTool(MapDocument* document);
    virtual ~DebugTestTool();

    static QString staticName();

protected:
    virtual void vKeyPress(QKeyEvent *e);
    virtual void vSelectedSetChanged();
    virtual void vDeactivate();

private:
    typedef QHash<SceneObject*, SceneObjectManipulator> ManipTable;
    void updateTableFromSet();
    void updateTableManipulators();
    void applyTableManipulators();

    ManipTable  m_ManipTable;
    QVector3D   m_vecTranslation;
};

#endif // DEBUGTESTTOOL_H
