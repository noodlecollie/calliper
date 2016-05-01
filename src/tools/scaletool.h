#ifndef SCALETOOL_H
#define SCALETOOL_H

#include "basetool.h"
#include "sceneobjectmanipulator.h"
#include <QHash>

class ScaleHandle;

class ScaleTool : public BaseTool
{
    Q_OBJECT
public:
    explicit ScaleTool(MapDocument *document);
    virtual ~ScaleTool();
    static QString staticName();

signals:

public slots:

protected:
    virtual void vActivate();
    virtual void vDeactivate();
    virtual void vKeyPress(QKeyEvent *e);
    virtual void vKeyRelease(QKeyEvent *e);
    virtual void vSelectedSetChanged();
    virtual void vMousePress(QMouseEvent *e);
    virtual void vMouseMove(QMouseEvent *e);
    virtual void vMouseRelease(QMouseEvent *e);

private:
    typedef QHash<SceneObject*, SceneObjectManipulator> ManipTable;

    void endMove();
    void updateHandleState();
    void updateTableFromSet();
    void updateTableManipulators();
    void commitTableManipulators();
    void clearTableManipulators();
    bool isAncestorInManipulatorTable(const SceneObject* obj) const;

    ScaleHandle*        m_pHandle;
    bool                m_bInMove;
    ManipTable          m_ManipTable;
    QVector3D           m_vecScale;
    QVector3D           m_vecOriginalHandlePos;
    QPoint              m_BeginDragPos;
    float               m_flHandleCamDist;
    int                 m_iAxisFlags;
    QList<QVector3D>    m_MovementAxes;
};

#endif // SCALETOOL_H
