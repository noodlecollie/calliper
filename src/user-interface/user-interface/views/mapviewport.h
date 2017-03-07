#ifndef MAPVIEWPORT_H
#define MAPVIEWPORT_H

#include "user-interface_global.h"
#include <QOpenGLWidget>
#include "model/scene/mapscene.h"
#include "model/scenerenderer/scenerenderer.h"
#include <QPointer>

namespace UserInterface
{
    class MapViewport : public QOpenGLWidget
    {
        Q_OBJECT
    public:
        MapViewport(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

        Model::MapScene* scene() const;
        void setScene(Model::MapScene* scene);

    signals:

    public slots:

    private:
        // To render we need a scene, a scene renderer and a shader palette.
        // The scene should be set by whatever has opened us, as one view should
        // correspond to one scene. The scene renderer can be held by us as it
        // takes our scene as input, but it will also need a render classifier
        // to be provided - I'm not sure yet whether the classifier should come
        // from the model or the rendering mode. The shader palette should
        // definitely come from the rendering mode.

        // There should be a shader palette store where enum values are mapped to
        // palettes - these would correspond to the shader side of a rendering mode.
        // The render classifier should probably come from the model, as the model
        // will know the significance of each different object ID.

        QPointer<Model::MapScene> m_pScene;
        Model::SceneRenderer m_SceneRenderer;
        Model::IRenderPassClassifier* m_pRenderPassClassifier;
    };
}

#endif // MAPVIEWPORT_H
