#ifndef MAPVIEWWINDOW_H
#define MAPVIEWWINDOW_H

#include "user-interface_global.h"
#include <QOpenGLWindow>
#include "model/global/resourceenvironment.h"
#include "model/scene/mapscene.h"
#include "model/camera/scenecamera.h"
#include "model/scenerenderer/scenerenderer.h"
#include "model/camera/cameracontroller.h"
#include "model/controller-adapters/keymap.h"
#include "model/controller-adapters/mouseeventmap.h"
#include "file-formats/vpk/vpkfilecollection.h"
#include "renderer/rendermodel/0-modellevel/rendermodel.h"
#include "model/filedatamodels/map/mapfiledatamodel.h"

namespace UserInterface
{
    class USERINTERFACESHARED_EXPORT MapViewWindow : public QOpenGLWindow
    {
        Q_OBJECT
    public:
        // Takes ownership of the classifier object.
        explicit MapViewWindow();
        virtual ~MapViewWindow();

        QString vpkPath() const;
        void setVpkPath(const QString& path);

        QString mapPath() const;
        void setMapPath(const QString& path);

        Model::MapFileDataModel* mapDataModel();
        const Model::MapFileDataModel* mapDataModel() const;

        Model::IRenderPassClassifier* renderPassClassifier();
        const Model::IRenderPassClassifier* renderPassClassifier() const;

        const FileFormats::VPKFileCollection& vpkFileCollection() const;

        void loadMap();
        void loadVpks();

    signals:
        void initialised();

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
        void initCameraController();
        void initKeyMap();
        void initMouseEventMap();

        QString m_strMapPath;
        QString m_strVpkPath;
        bool m_bInitialised;

        Model::MapFileDataModel* m_pVmfData;

        Renderer::RenderModel* m_pRenderer;

        Model::CameraController* m_pCameraController;
        Model::KeyMap* m_pKeyMap;
        Model::MouseEventMap* m_pMouseEventMap;

        FileFormats::VPKFileCollection m_VpkFiles;
    };
}

#endif // MAPVIEWWINDOW_H
