#ifndef TRANSLATIONHANDLE_H
#define TRANSLATIONHANDLE_H

#include "uimanipulator.h"

class TranslationHandle : public UIManipulator
{
public:
    explicit TranslationHandle(SceneObject* parent = 0);
    virtual ~TranslationHandle();
    virtual SceneObject* clone() const;

    virtual void draw(ShaderStack* stack);
    static int axisFlagsFromPickColor(QRgb colour);

protected:
    explicit TranslationHandle(const TranslationHandle &cloneFrom);

private:
    void drawNormal();
    void drawForPicking(ShaderStack* stack);
    void build();

    int m_iShaftOffset;
    int m_iXOffset;
    int m_iYOffset;
    int m_iZOffset;
    int m_iXYOffset;
    int m_iYZOffset;
    int m_iXZOffset;
    int m_iAxisSectionLength;
    int m_iPlaneSectionLength;
};

#endif // TRANSLATIONHANDLE_H
