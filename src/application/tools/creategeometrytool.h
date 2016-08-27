#ifndef CREATEGEOMETRYTOOL_H
#define CREATEGEOMETRYTOOL_H

#include "uimanipulatortool.h"
#include <QVector3D>
#include "ray3d.h"

class BlockCreationHandle;
class SceneCamera;
class Crosshair3D;
class UIBlockCreationHandle;

class CreateGeometryTool : public BaseTool
{
    Q_OBJECT
public:
    explicit CreateGeometryTool(MapDocument *document);
    virtual ~CreateGeometryTool();
    static QString staticName();

    virtual void vActivate();
    virtual void vDeactivate();
    virtual void vMousePress(QMouseEvent *e);
    virtual void vMouseRelease(QMouseEvent *e);
    virtual void vMouseMove(QMouseEvent *e);
    virtual void vMouseMoveHover(QMouseEvent *e);
    virtual void vKeyPress(QKeyEvent *e);
    virtual void vWheel(QWheelEvent *e);

signals:

public slots:

private:
    void endDrag();
    static bool rayIntersectsZ0Plane(SceneCamera* camera, const Ray3D &ray, QVector3D &intersection);
    void updateManipulatorBounds(bool endOfDrag = false);
    void updateCrosshairVisibility(const QPoint &mousePos);
    void clearManipulator();
    void updateBoundsDepth();
    void createBlockBrush();
    void createScaleHandles();
    void destroyScaleHandles();

    BlockCreationHandle*    m_pManipulator;
    Crosshair3D*            m_pCrosshair;
    UIBlockCreationHandle*  m_pUIBox;

    bool                    m_bInDrag;
    QPoint                  m_PosDragBegin;
    QPoint                  m_PosDragCurrent;
    QVector3D               m_vecDragBegin;
    QVector3D               m_vecDragBeginClamped;
    QVector3D               m_vecDragCurrent;
    QVector3D               m_vecDragCurrentClamped;
    SceneObject*            m_pBeginDragTarget;
    float                   m_flBlockDepth;
};

#endif // CREATEGEOMETRYTOOL_H