#ifndef SCALETOOL_H
#define SCALETOOL_H

#include "uimanipulatortool.h"

class ScaleHandle;

class ScaleTool : public UIManipulatorTool
{
    Q_OBJECT
public:
    explicit ScaleTool(MapDocument *document);
    virtual ~ScaleTool();
    static QString staticName();

signals:

public slots:

protected:
    virtual UIManipulator* constructManipulator();
    virtual void updateManipulatorFromMouseMove(QMouseEvent *e);
    virtual void updateManipulatorForMouseRelease(QMouseEvent *);
    virtual void updateManipulator();
    virtual void updateSceneObjectManipulator(SceneObject *, SceneObjectManipulator &manip);
    virtual void commitSceneObjectManipulator(SceneObject *, SceneObjectManipulator &manip);
    virtual void startManipulatorHover(QRgb pickColour);
    virtual void endManipulatorHover();
    virtual void updateManipulatorHover(QRgb pickColour);
};

#endif // SCALETOOL_H
