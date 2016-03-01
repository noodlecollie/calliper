#ifndef TRANSLATIONTOOL_H
#define TRANSLATIONTOOL_H

#include "basetool.h"
#include "sceneobjectmanipulator.h"

class TranslationHandle;

class TranslationTool : public BaseTool
{
public:
    explicit TranslationTool(MapDocument* document);
    virtual ~TranslationTool();
    static QString staticName();

protected:
    virtual void vKeyPress(QKeyEvent *e);
    virtual void vKeyRelease(QKeyEvent *e);
    virtual void vSelectedSetChanged();
    virtual void vActivate();
    virtual void vDeactivate();
    virtual void vMousePress(QMouseEvent *e);
    virtual void vMouseMove(QMouseEvent *e);
    virtual void vMouseRelease(QMouseEvent *);

private:
    typedef QHash<SceneObject*, SceneObjectManipulator> ManipTable;

    void endMove();
    void updateHandleState();
    void updateTableFromSet();
    void updateTableManipulators();
    void commitTableManipulators();
    void clearTableManipulators();
    bool isAncestorInManipulatorTable(const SceneObject* obj) const;

    bool				m_bInMove;
    TranslationHandle*	m_pHandle;
    QVector3D           m_vecOriginalHandlePos;
    QPoint              m_BeginDragPos;
    float               m_flHandleCamDist;
    int                 m_iAxisFlags;
    QList<QVector3D>    m_MovementAxes;
    ManipTable          m_ManipTable;
    QVector3D           m_vecCurrentWorldTranslation;
};

#endif // TRANSLATIONTOOL_H
