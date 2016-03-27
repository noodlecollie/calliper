#ifndef SCALETOOL_H
#define SCALETOOL_H

#include "basetool.h"
#include "sceneobjectmanipulator.h"

class ScaleTool : public BaseTool
{
    Q_OBJECT
public:
    explicit ScaleTool(MapDocument *document);
    virtual ~ScaleTool();
    static QString staticName();

signals:

public slots:
};

#endif // SCALETOOL_H
