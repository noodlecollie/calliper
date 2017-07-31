#include "mainwindow.h"

#include <QtDebug>
#include <QMessageBox>
#include <QtGlobal>
#include <QMap>

#include "calliperutil/opengl/openglerrors.h"
#include "calliperutil/opengl/openglhelpers.h"

#include "model/genericbrush/genericbrush.h"

#include "model-loaders/vtf/vtfloader.h"

#include "file-formats/vpk/vpkindextreerecord.h"

MainWindow::MainWindow()
    : UserInterface::MapViewWindow(),
    m_iPlaceholderMaterial(0)
{
    connect(this, SIGNAL(initialised()), this, SLOT(init()));
    resize(640, 480);
}

MainWindow::~MainWindow()
{
}

void MainWindow::initShaders()
{
}

void MainWindow::initTextures()
{
}

void MainWindow::initMaterials()
{
}

void MainWindow::initLocalOpenGlSettings()
{
    GL_CURRENT_F;

    GLTRY(f->glEnable(GL_CULL_FACE));
    GLTRY(f->glCullFace(GL_BACK));

    GLTRY(f->glEnable(GL_DEPTH_TEST));
    GLTRY(f->glDepthFunc(GL_LESS));
}

void MainWindow::processBrushes()
{
    using namespace Model;

    QList<GenericBrush*> brushes = mapDataModel()->scene()->rootObject()->findChildren<GenericBrush*>();

    foreach ( GenericBrush* brush, brushes )
    {
        float rand = (float)qrand()/(float)RAND_MAX;
        rand = 0.5f + (0.5f * rand);
        unsigned char randChar = (unsigned char)(rand * 0xff);
        QRgb col = (0xff << 24) | (randChar << 16) | (randChar << 8) | randChar;
        brush->setColor(QColor::fromRgb(col));

        for ( int i = 0; i < brush->brushFaceCount(); i++ )
        {
            GenericBrushFace* face = brush->brushFaceAt(i);
            face->texturePlane()->setMaterialId(m_iPlaceholderMaterial);
        }
    }
}

void MainWindow::importTextures()
{

}

void MainWindow::init()
{
    loadMap();
}
