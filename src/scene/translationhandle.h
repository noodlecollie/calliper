#ifndef TRANSLATIONHANDLE_H
#define TRANSLATIONHANDLE_H

#include "uimanipulator.h"

class TranslationHandle : public UIManipulator
{
	Q_OBJECT
public:
    explicit TranslationHandle(SceneObject* parent = 0);
    virtual ~TranslationHandle();
    virtual SceneObject* clone() const;

    virtual void draw(ShaderStack* stack);
    static int axisFlagsFromPickColor(QRgb colour);

protected:
    explicit TranslationHandle(const TranslationHandle &cloneFrom);

private:
    void build();
};

#endif // TRANSLATIONHANDLE_H
