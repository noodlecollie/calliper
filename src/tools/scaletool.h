#ifndef SCALETOOL_H
#define SCALETOOL_H

#include "basetool.h"
#include "sceneobjectmanipulator.h"

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
    void endMove();

    ScaleHandle*    m_pHandle;
    bool            m_bInMove;
};

#endif // SCALETOOL_H
