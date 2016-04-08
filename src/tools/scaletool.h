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

private:
    ScaleHandle*    m_pHandle;
};

#endif // SCALETOOL_H
