#ifndef TRANSLATIONHANDLE_H
#define TRANSLATIONHANDLE_H

#include "uimanipulator.h"

class TranslationHandle : public UIManipulator
{
	Q_OBJECT
    friend class BaseScene;
public:
    virtual SceneObject* clone() const;

    virtual void draw(ShaderStack* stack);

protected:
    explicit TranslationHandle(BaseScene* scene, SceneObject* parent);
    TranslationHandle(const TranslationHandle &cloneFrom);
    virtual ~TranslationHandle();

private:
    void build();
};

#endif // TRANSLATIONHANDLE_H
