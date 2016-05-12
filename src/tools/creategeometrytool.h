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

signals:

public slots:

private:
    BlockCreationHandle*    m_pManipulator;
};

#endif // CREATEGEOMETRYTOOL_H
