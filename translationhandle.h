#ifndef TRANSLATIONHANDLE_H
#define TRANSLATIONHANDLE_H

#include "sceneobject.h"

class TranslationHandle : public SceneObject
{
public:
    TranslationHandle(SceneObject* parent = 0);
    virtual ~TranslationHandle();

    virtual void draw(ShaderStack* stack);

    static const QColor xAxisPickColor;
    static const QColor yAxisPickColor;
    static const QColor zAxisPickColor;

private:
    void drawNormal();
    void drawForPicking(ShaderStack* stack);
    void build();

    int m_iShaftOffset;
    int m_iXOffset;
    int m_iYOffset;
    int m_iZOffset;
};

#endif // TRANSLATIONHANDLE_H
