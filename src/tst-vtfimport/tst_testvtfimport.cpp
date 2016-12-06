#include <QString>
#include <QtTest>
#include "views/mapviewwindow.h"
#include "VTFLib/src/VTFFile.h"

class SimpleClassifier : public Model::IRenderPassClassifier
{
public:
    virtual int classify(quint32) const override
    {
        return 0;
    }
};

class TestVtfImport : public QObject
{
    Q_OBJECT

public:
    TestVtfImport();

private:
    void initGl()
    {
        QSurfaceFormat format;
        format.setMajorVersion(4);
        format.setMinorVersion(1);
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setRenderableType(QSurfaceFormat::OpenGL);
        format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
        format.setSamples(2);
        format.setDepthBufferSize(24);
        format.setRedBufferSize(8);
        format.setGreenBufferSize(8);
        format.setBlueBufferSize(8);
        format.setAlphaBufferSize(0);
        format.setStencilBufferSize(8);
        QSurfaceFormat::setDefaultFormat(format);
    }

private Q_SLOTS:
    void testCase1();
};

TestVtfImport::TestVtfImport()
{

}

void TestVtfImport::testCase1()
{
    using namespace HighLevelConvenience;

    initGl();

    MapViewWindow win(new SimpleClassifier());
    win.show();

    win.makeCurrent();

    Model::TextureStore* textureStore = win.textureStore();
    Renderer::OpenGLTexturePointer texture = textureStore->createEmptyTexture("testPath");

    QFile sourceFile(":/rockground003");
    if ( !sourceFile.open(QIODevice::ReadOnly) )
    {
        QVERIFY2(false, "Failed to open texture source file.");
    }

    QByteArray vtfData = sourceFile.readAll();
    sourceFile.close();

    VTFLib::CVTFFile vtfFile;
    if ( !vtfFile.Load(vtfData.data(), vtfData.size(), false) )
    {
        QVERIFY2(false, "Failed to load VTF data.");
    }

    if ( !texture->create() )
    {
        QVERIFY2(false, "Failed to create texture");
    }

    texture->setFormat(QOpenGLTexture::RGBA_DXT1);
    texture->setSize(vtfFile.GetWidth(), vtfFile.GetHeight());
    texture->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt32);
    texture->setCompressedData(vtfFile.GetWidth() * vtfFile.GetHeight() * sizeof(quint32), vtfFile.GetData(0,0,0,0));

    win.doneCurrent();
}

QTEST_APPLESS_MAIN(TestVtfImport)

#include "tst_testvtfimport.moc"
