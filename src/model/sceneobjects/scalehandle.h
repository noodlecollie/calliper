#ifndef SCALEHANDLE_H
#define SCALEHANDLE_H

#include "model_global.h"
#include "uimanipulator.h"

namespace NS_MODEL
{
    class ScaleHandle : public UIManipulator
    {
        Q_OBJECT
        friend class BaseScene;
    public:
        void draw(ShaderStack *stack);

        void rescaleHandle(const QVector3D scale);

    signals:

    public slots:

    protected:
        explicit ScaleHandle(BaseScene* scene, SceneObject *parent);
        ScaleHandle(const ScaleHandle &cloneFrom);
        virtual ~ScaleHandle();

    private:
        void build(const QVector3D &scale = QVector3D(1,1,1));
    };
}

#endif // SCALEHANDLE_H
