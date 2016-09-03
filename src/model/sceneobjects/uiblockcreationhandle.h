#ifndef UIWIREFRAMEBOX_H
#define UIWIREFRAMEBOX_H

#include "model_global.h"
#include "uimanipulator.h"

#define SCALE_HANDLE_COUNT 20

namespace NS_MODEL
{
    class AxisDragHandle;

    class MODELSHARED_EXPORT UIBlockCreationHandle : public UIManipulator
    {
        Q_OBJECT
        friend class BaseScene;
    public:
        BoundingBox bounds() const;
        void setBounds(const BoundingBox &bounds);
        void setBounds(const QVector3D &min, const QVector3D &max);
        virtual void draw(ShaderStack *stack);

    protected:
        explicit UIBlockCreationHandle(BaseScene* scene, SceneObject *parent);
        UIBlockCreationHandle(const UIBlockCreationHandle &cloneFrom);
        virtual ~UIBlockCreationHandle();

    private:
        void initDefaults();
        void rebuildGeometry();
        void createScaleHandles();
        void updateScaleHandles();
        void hideAllScaleHandles();

        BoundingBox     m_Bounds;
        bool            m_bRebuildGeometry;
        AxisDragHandle* m_pScaleHandles[SCALE_HANDLE_COUNT];
    };
}

#endif // UIWIREFRAMEBOX_H
