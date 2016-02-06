#ifndef ORIGINMARKER_H
#define ORIGINMARKER_H

#include "sceneobject.h"

class OriginMarker : public SceneObject
{
    Q_OBJECT
public:
    explicit OriginMarker(SceneObject* parent = 0);

    virtual bool editable() const;
    virtual SceneObject* clone() const;

protected:
    explicit OriginMarker(const OriginMarker &cloneFrom);

private:
    void constructGeometry();
};

#endif // ORIGINMARKER_H
