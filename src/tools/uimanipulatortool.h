#ifndef UIMANIPULATORTOOL_H
#define UIMANIPULATORTOOL_H

#include "basetool.h"
#include "sceneobjectmanipulator.h"
#include "spatialsnapshot.h"

class UIManipulator;
class Viewport;

class UIManipulatorTool : public BaseTool
{
    Q_OBJECT
public:
    explicit UIManipulatorTool(const QString &name, MapDocument* document);
    virtual ~UIManipulatorTool();

signals:

public slots:

protected:
    typedef QHash<SceneObject*, SceneObjectManipulator> ManipTable;

    virtual void vActivate();
    virtual void vDeactivate();
    virtual void vKeyPress(QKeyEvent *e);
    virtual void vKeyRelease(QKeyEvent *e);
    virtual void vSelectedSetChanged();
    virtual void vMousePress(QMouseEvent *e);
    virtual void vMouseMove(QMouseEvent *e);
    virtual void vMouseRelease(QMouseEvent *e);

    void endMove();
    void updateTableFromSet();
    void updateTableManipulators();
    void commitTableManipulators();
    void clearTableManipulators();
    bool isAncestorInManipulatorTable(const SceneObject* obj) const;

    virtual UIManipulator* constructManipulator() = 0;
    virtual void updateManipulatorFromMouseMove(QMouseEvent* e) = 0;
    virtual void updateManipulatorForMouseRelease(QMouseEvent* e) = 0;
    virtual void updateManipulator() = 0;
    virtual void updateSceneObjectManipulator(SceneObject* obj, SceneObjectManipulator &manip) = 0;
    virtual void commitSceneObjectManipulator(SceneObject* obj, SceneObjectManipulator &manip) = 0;

    UIManipulator*      m_pManipulator;
    bool                m_bInMove;
    ManipTable          m_ManipTable;
    SpatialSnapshot     m_ManipulatorOriginalOrientation;
    SpatialSnapshot     m_Transformation;
    QPoint              m_BeginDragPos;
    float               m_flHandleCamDist;
    int                 m_iAxisFlags;
    QList<QVector3D>    m_MovementAxes;
};

#endif // UIMANIPULATORTOOL_H
