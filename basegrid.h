#ifndef BASEGRID_H
#define BASEGRID_H

#include "sceneobject.h"
#include <QColor>
#include <QList>
#include <QPair>
#include "boundingbox.h"

class BaseGrid : public SceneObject
{
    Q_OBJECT
public:
    explicit BaseGrid(SceneObject* parent = 0);

    virtual bool editable() const;
    virtual void draw(ShaderStack *stack);

private:
    void setUpGeometry();
    void drawOriginLines(ShaderStack* stack, const BoundingBox &bbox);

    QColor  m_colMajor;
    QColor  m_colMinor;
    QColor  m_colOrigin;
    QColor  m_colStd;

    enum Component
    {
        Origin = 0,
        Major,
        Minor,
        Std
    };

    QList<QPair<int,int> >  m_DrawOffsets;  // First = offset (in vertices), Second = count
};

#endif // BASEGRID_H
