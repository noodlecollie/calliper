#ifndef MAPVIEWWINDOW_H
#define MAPVIEWWINDOW_H

#include "high-level-convenience_global.h"
#include <QOpenGLWindow>
#include "stores/shaderstore.h"
#include "stores/texturestore.h"
#include "stores/materialstore.h"
#include "scene/basicscene.h"
#include "camera/scenecamera.h"
#include "scenerenderer/scenerenderer.h"
#include "camera/cameracontroller.h"
#include "controller-adapters/keymap.h"
#include "controller-adapters/mouseeventmap.h"
#include "vpk/vpkfilecollection.h"
#include "rendermodel/0-modellevel/rendermodel.h"

namespace HighLevelConvenience
{
    class HIGHLEVELCONVENIENCESHARED_EXPORT MapViewWindow : public QOpenGLWindow
    {
        Q_OBJECT

    public:
        // Takes ownership of the classifier object.
        explicit MapViewWindow(Model::IRenderPassClassifier* classifier);
        ~MapViewWindow();

        QString vpkPath() const;
        void setVpkPath(const QString& path);

        QString mapPath() const;
        void setMapPath(const QString& path);

        Model::ShaderPalette defaultShaderPalette() const;
        void setDefaultShaderPalette(const Model::ShaderPalette& palette);

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
        Model::ShaderPalette m_DefaultShaderPalette;

        Model::CameraController* m_pCameraController;
        Model::KeyMap* m_pKeyMap;
        Model::MouseEventMap* m_pMouseEventMap;

        FileFormats::VPKFileCollection m_VpkFiles;
    };
}

#endif // MAPVIEWWINDOW_H
