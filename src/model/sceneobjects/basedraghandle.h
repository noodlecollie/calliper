#ifndef BASEDRAGHANDLE_H
#define BASEDRAGHANDLE_H

#include "model_global.h"
#include "uimanipulator.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT BaseDragHandle : public UIManipulator
    {
        friend class BaseScene;
        Q_OBJECT
    public:
        virtual void draw(ShaderStack *stack);

    protected:
        explicit BaseDragHandle(BaseScene* scene, SceneObject *parent, const QString &iconTexture);
        BaseDragHandle(const BaseDragHandle &cloneFrom);
        virtual ~BaseDragHandle();

    private:
        void buildSprite();

        QString     m_szIconTexture;
    };
}

#endif // BASEDRAGHANDLE_H
