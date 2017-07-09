#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "rendersystem/endpoints/rendermodelstoreendpoint.h"
#include "rendersystem/endpoints/framebufferstoreendpoint.h"
#include "rendersystem/interface-classes/geometry/geometrybuilder.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_nRenderModelId(RenderSystem::RenderModelDefs::INVALID_RENDER_MODEL_ID),
    m_nFrameBufferId(RenderSystem::FrameBufferDefs::INVALID_FRAME_BUFFER_ID)
{
    using namespace RenderSystem;

    ui->setupUi(this);

    FrameBufferStoreEndpoint::FrameBufferStoreAccessor frameBufferStore = FrameBufferStoreEndpoint::frameBufferStore();
    RenderModelStoreEndpoint::RenderModelStoreAccessor renderModelStore = RenderModelStoreEndpoint::renderModelStore();

    m_nFrameBufferId = frameBufferStore->createFrameBuffer(QSize(128, 128));
    m_nRenderModelId = renderModelStore->createRenderModel();

    GeometryBuilder builder(1, 0, QMatrix4x4());
    GeometryBuilder::GeometrySectionPointer section = builder.createNewSection();

    section->addPosition(QVector3D(-1, -1, 0));
    section->addPosition(QVector3D(1, -1, 0));
    section->addPosition(QVector3D(0, 1, 0));

    section->addColor(QColor::fromRgb(0xff0000ff));
    section->addColor(QColor::fromRgb(0xff0000ff));
    section->addColor(QColor::fromRgb(0xff0000ff));

    section->addIndexTriangle(0, 1, 2);

    renderModelStore->addGeometry(m_nRenderModelId, builder);
    renderModelStore->draw(m_nRenderModelId,
                           m_nFrameBufferId,
                           ShaderDefs::BarebonesRenderMode,
                           QMatrix4x4(),
                           QMatrix4x4());

    frameBufferStore->save(m_nFrameBufferId, "/Users/vesper/Desktop/temp.png");
}

MainWindow::~MainWindow()
{
    using namespace RenderSystem;

    FrameBufferStoreEndpoint::FrameBufferStoreAccessor frameBufferStore = FrameBufferStoreEndpoint::frameBufferStore();
    RenderModelStoreEndpoint::RenderModelStoreAccessor renderModelStore = RenderModelStoreEndpoint::renderModelStore();

    renderModelStore->removeRenderModel(m_nRenderModelId);
    m_nRenderModelId = RenderModelDefs::INVALID_RENDER_MODEL_ID;

    frameBufferStore->removeFrameBuffer(m_nFrameBufferId);
    m_nFrameBufferId = FrameBufferDefs::INVALID_FRAME_BUFFER_ID;

    delete ui;
}
