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

signals:

public slots:

protected:
    explicit ScaleHandle(const ScaleHandle &cloneFrom);
};

#endif // SCALEHANDLE_H
