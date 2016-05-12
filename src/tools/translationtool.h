#ifndef TRANSLATIONTOOL_H
#define TRANSLATIONTOOL_H

#include "uimanipulatortool.h"

class TranslationHandle;

class TranslationTool : public UIManipulatorTool
{
    Q_OBJECT
public:
    explicit TranslationTool(MapDocument* document);
    virtual ~TranslationTool();
    static QString staticName();

protected:
    virtual UIManipulator* constructManipulator();
    virtual void updateManipulatorFromMouseMove(QMouseEvent *e);
    virtual void updateManipulatorForMouseRelease(QMouseEvent *);
    virtual void updateManipulator();
    virtual void updateSceneObjectManipulator(SceneObject *, SceneObjectManipulator &manip);
    virtual void commitSceneObjectManipulator(SceneObject *, SceneObjectManipulator &manip);
};

#endif // TRANSLATIONTOOL_H
