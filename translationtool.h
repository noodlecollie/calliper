#ifndef TRANSLATIONTOOL_H
#define TRANSLATIONTOOL_H

#include "basetool.h"

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
    void endMove();
    void updateHandleState();
    void updateTableFromSet();

    bool				m_bInMove;
    TranslationHandle*	m_pHandle;
    QVector3D           m_vecOriginalHandlePos;
    QPoint              m_BeginDragPos;
    float               m_flHandleCamDist;
    int                 m_iAxisFlags;
    QList<QVector3D>    m_MovementAxes;
};

#endif // TRANSLATIONTOOL_H
