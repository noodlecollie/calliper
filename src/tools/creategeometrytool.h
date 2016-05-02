#ifndef CREATEGEOMETRYTOOL_H
#define CREATEGEOMETRYTOOL_H

#include "basetool.h"

class CreateGeometryTool : public BaseTool
{
    Q_OBJECT
public:
    explicit CreateGeometryTool(MapDocument *document);
    virtual ~CreateGeometryTool();
    static QString staticName();

signals:

public slots:
};

#endif // CREATEGEOMETRYTOOL_H
