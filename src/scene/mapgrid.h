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
    friend class BaseScene;
public:
    virtual bool editable() const;
    virtual bool isBackground() const;
    virtual void draw(ShaderStack *stack);
    virtual SceneObject* clone() const;

    int gridPower() const;
    void setGridPower(int power);
    void incrementGridPower();
    void decrementGridPower();
    unsigned int gridMultiple() const;

signals:
    void gridPowerChanged();

protected:
    explicit MapGrid(BaseScene* scene, SceneObject* parent);
    virtual ~MapGrid();

private:
    void setUpGeometry();
    void drawOriginLines(ShaderStack* stack, const BoundingBox &bbox, GeometryDataPointer &geom);
    void drawMajorLines(ShaderStack* stack, const BoundingBox &bbox, GeometryDataPointer &geom);
    void drawMinorLines(ShaderStack* stack, const BoundingBox &bbox, GeometryDataPointer &geom);
    void drawStandardLines(ShaderStack* stack, const BoundingBox &bbox, GeometryDataPointer &geom);
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
