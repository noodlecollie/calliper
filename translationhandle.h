#ifndef TRANSLATIONHANDLE_H
#define TRANSLATIONHANDLE_H

#include "sceneobject.h"

class TranslationHandle : public SceneObject
{
    Q_OBJECT
public:
    explicit TranslationHandle(SceneObject* parent = 0);
    virtual ~TranslationHandle();

    virtual void draw(ShaderStack *stack);
    virtual bool editable() const;
    virtual bool screenSpace() const;

private:
    void buildGeometry();

    int m_iSegmentSize;
};

#endif // TRANSLATIONHANDLE_H