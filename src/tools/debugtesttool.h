#ifndef DEBUGTESTTOOL_H
#define DEBUGTESTTOOL_H

#include "basetool.h"
#include <QHash>
#include "sceneobjectmanipulator.h"
#include "callipermath.h"

class SceneObject;
class TranslationHandle;
class SceneCamera;

class DebugTestTool : public BaseTool
{
public:
	explicit DebugTestTool(MapDocument* document);
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

};

#endif // DEBUGTESTTOOL_H
