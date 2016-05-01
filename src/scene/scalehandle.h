#ifndef SCALEHANDLE_H
#define SCALEHANDLE_H

#include "uimanipulator.h"

class ScaleHandle : public UIManipulator
{
    Q_OBJECT
public:
    explicit ScaleHandle(SceneObject *parent = 0);
    virtual ~ScaleHandle();
    virtual SceneObject* clone() const;
    void draw(ShaderStack *stack);

    void rescaleHandle(const QVector3D scale);

signals:

public slots:

protected:
    explicit ScaleHandle(const ScaleHandle &cloneFrom);

private:
    void build(const QVector3D &scale = QVector3D(1,1,1));
};

#endif // SCALEHANDLE_H
