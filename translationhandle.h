#ifndef TRANSLATIONHANDLE_H
#define TRANSLATIONHANDLE_H

#include "sceneobject.h"

class TranslationHandle : public SceneObject
{
public:
    TranslationHandle(SceneObject* parent = 0);
    virtual ~TranslationHandle();

    virtual void draw(ShaderStack* stack);

private:
    int m_iShaftOffset;
};

#endif // TRANSLATIONHANDLE_H
