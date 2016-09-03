#ifndef TRANSLATIONHANDLE_H
#define TRANSLATIONHANDLE_H

#include "model_global.h"
#include "uimanipulator.h"
#include <QVector>

namespace NS_MODEL
{
    class MODELSHARED_EXPORT TranslationHandle : public UIManipulator
    {
        Q_OBJECT
        friend class BaseScene;
    public:
        virtual void draw(ShaderStack* stack);

        // Pass -1 to set no axis.
        // See Math::AxisIdentifier for axes.
        void setHoverAxis(int axis);
        void setHoverSection(QRgb pickColour);

    protected:
        explicit TranslationHandle(BaseScene* scene, SceneObject* parent);
        TranslationHandle(const TranslationHandle &cloneFrom);
        virtual ~TranslationHandle();

    private:
        void build();

        QVector<int>    m_ArrowSections;
        QVector<int>    m_ShaftSections;
    };
}

#endif // TRANSLATIONHANDLE_H
