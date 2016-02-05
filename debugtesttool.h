#ifndef DEBUGTESTTOOL_H
#define DEBUGTESTTOOL_H

#include "basetool.h"
#include <QHash>
#include "sceneobjectmanipulator.h"

class SceneObject;
class TranslationHandle;

class DebugTestTool : public BaseTool
{
public:
    DebugTestTool(MapDocument* document);
    virtual ~DebugTestTool();

    static QString staticName();

protected:
    virtual void vKeyPress(QKeyEvent *e);
    virtual void vSelectedSetChanged();
    virtual void vActivate();
    virtual void vDeactivate();
    virtual void vMousePress(QMouseEvent *e);
    virtual void vMouseMove(QMouseEvent *e);
    virtual void vMouseRelease(QMouseEvent *);

private:
    typedef QHash<SceneObject*, SceneObjectManipulator> ManipTable;
    void updateTableFromSet();
    void updateTableManipulators();
    void applyTableManipulators();

    ManipTable  m_ManipTable;
    QVector3D   m_vecTranslation;

    TranslationHandle* m_pHandle;
    QPoint m_BeginPos;
    bool m_bInMove;
    QVector3D m_vecOriginalHandlePos;
    QVector3D m_vecMovementAxis;
    float m_flHandeCamDist;
};

#endif // DEBUGTESTTOOL_H
