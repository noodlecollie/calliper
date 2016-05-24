#ifndef CREATEGEOMETRYTOOL_H
#define CREATEGEOMETRYTOOL_H

#include "uimanipulatortool.h"

class BlockCreationHandle;

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

signals:

public slots:

private:
    void endDrag();

    BlockCreationHandle*    m_pManipulator;

    bool                    m_bInDrag;
    QPoint                  m_PosDragBegin;
};

#endif // CREATEGEOMETRYTOOL_H
