#ifndef BASEGRID_H
#define BASEGRID_H

#include "sceneobject.h"
#include <QColor>
#include <QList>
#include <QPair>
#include "boundingbox.h"

class MapGrid : public SceneObject
{
    Q_OBJECT
public:
    explicit MapGrid(SceneObject* parent = 0);

    virtual bool editable() const;
    virtual void draw(ShaderStack *stack);

    int gridPower() const;
    void setGridPower(int power);
    void incrementGridPower();
    void decrementGridPower();
    float gridMultiple() const;

private:
    void setUpGeometry();
    void drawOriginLines(ShaderStack* stack, const BoundingBox &bbox);
    void drawMajorLines(ShaderStack* stack, const BoundingBox &bbox);
    void drawMinorLines(ShaderStack* stack, const BoundingBox &bbox);
    void drawStandardLines(ShaderStack* stack, const BoundingBox &bbox);
    int limitGridPower(float camWorldZ) const;
    static int stdLineVertCount(int power);

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
    int m_iPowerTwo;
};

#endif // BASEGRID_H
