#ifndef MAPVIEWWINDOW_H
#define MAPVIEWWINDOW_H

#include "user-interface_global.h"
#include <QOpenGLWindow>
#include "model/stores/shaderstore.h"
#include "model/stores/texturestore.h"
#include "model/stores/materialstore.h"
#include "model/scene/basicscene.h"
#include "model/camera/scenecamera.h"
#include "model/scenerenderer/scenerenderer.h"
#include "model/camera/cameracontroller.h"
#include "model/controller-adapters/keymap.h"
#include "model/controller-adapters/mouseeventmap.h"
#include "file-formats/vpk/vpkfilecollection.h"
#include "renderer/rendermodel/0-modellevel/rendermodel.h"

namespace UserInterface
{
    class USERINTERFACESHARED_EXPORT MapViewWindow : public QOpenGLWindow
    {
        Q_OBJECT
    public:
        // Takes ownership of the classifier object.
        explicit MapViewWindow(Model::IRenderPassClassifier* classifier);
        virtual ~MapViewWindow();

        QString vpkPath() const;
        void setVpkPath(const QString& path);

        QString mapPath() const;
        void setMapPath(const QString& path);

        Model::ShaderPalette& shaderPalette();
        const Model::ShaderPalette& shaderPalette() const;

        Model::ShaderStore* shaderStore();
        const Model::ShaderStore* shaderStore() const;

        Model::TextureStore* textureStore();
        const Model::TextureStore* textureStore() const;

        Model::MaterialStore* materialStore();
        const Model::MaterialStore* materialStore() const;

        Model::IRenderPassClassifier* renderPassClassifier();
        const Model::IRenderPassClassifier* renderPassClassifier() const;

        Model::Scene* scene();
        const Model::Scene* scene() const;

        Model::SceneCamera* sceneCamera();
        const Model::SceneCamera* sceneCamera() const;

        const FileFormats::VPKFileCollection& vpkFileCollection() const;

        void loadMap();
        void loadVpks();

    protected:
        virtual void initializeGL() override;
        virtual void paintGL() override;
        virtual void resizeGL(int w, int h) override;
        virtual void mousePressEvent(QMouseEvent *e) override;
        virtual void mouseReleaseEvent(QMouseEvent *e) override;

        virtual void initShaders();
        virtual void initTextures();
        virtual void initMaterials();
        virtual void initLocalOpenGlSettings();

    private:
        void destroy();
        void initRenderer();
        void initScene();
        void initSceneRenderer();
        void initCameraController();
        void initKeyMap();
        void initMouseEventMap();

        QString m_strMapPath;
        QString m_strVpkPath;

        Renderer::RenderModel* m_pRenderer;

        Model::ShaderStore* m_pShaderStore;
        Model::TextureStore* m_pTextureStore;
        Model::MaterialStore* m_pMaterialStore;

        Model::BasicScene* m_pScene;
        Model::SceneCamera* m_pCamera;
        Model::IRenderPassClassifier* m_pRenderPassClassifier;
        Model::SceneRenderer* m_pSceneRenderer;
        Model::ShaderPalette m_ShaderPalette;

        Model::CameraController* m_pCameraController;
        Model::KeyMap* m_pKeyMap;
        Model::MouseEventMap* m_pMouseEventMap;

        FileFormats::VPKFileCollection m_VpkFiles;
    };
}

#endif // MAPVIEWWINDOW_H
