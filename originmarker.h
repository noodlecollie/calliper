#ifndef ORIGINMARKER_H
#define ORIGINMARKER_H

#include "sceneobject.h"

class OriginMarker : public SceneObject
{
    Q_OBJECT
public:
    explicit OriginMarker(SceneObject* parent = 0);

    virtual bool editable() const;
};

#endif // ORIGINMARKER_H
