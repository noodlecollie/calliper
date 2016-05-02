#ifndef TRANSLATIONTOOL_H
#define TRANSLATIONTOOL_H

#include "uimanipulatortool.h"

class TranslationHandle;

class TranslationTool : public UIManipulatorTool
{
public:
    explicit TranslationTool(MapDocument* document);
    virtual ~TranslationTool();
    static QString staticName();

protected:
    virtual UIManipulator* constructManipulator();
    virtual void updateManipulatorFromMouseMove(QMouseEvent *e, Viewport* viewport);
    virtual void updateManipulator();
    virtual void updateSceneObjectManipulator(SceneObject *, SceneObjectManipulator &manip);
    virtual void commitSceneObjectManipulator(SceneObject *, SceneObjectManipulator &manip);
};

#endif // TRANSLATIONTOOL_H
