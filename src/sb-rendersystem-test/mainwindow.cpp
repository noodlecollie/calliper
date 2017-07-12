#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "rendersystem/endpoints/rendermodelstoreendpoint.h"
#include "rendersystem/endpoints/framebufferstoreendpoint.h"
#include "rendersystem/endpoints/materialstoreendpoint.h"
#include "rendersystem/interface-classes/geometry/geometrybuilder.h"

namespace
{
    void createTriangles(RenderSystem::GeometryBuilder& builder, int dim, int max = -1)
    {
        using namespace RenderSystem;

        if ( max < 0 )
        {
            max = dim * dim;
        }

        const float triangleDelta = 2.0f / static_cast<float>(dim);

        for ( int x = 0; x < dim; ++x )
        {
            for ( int y = 0; y < dim; ++y )
            {
                GeometryBuilder::GeometrySectionPointer section = builder.createNewSection();

                const float minX = -1 + (x * triangleDelta);
                const float maxX = minX + triangleDelta;
                const float minY = -1 + (y * triangleDelta);
                const float maxY = minY + triangleDelta;

                section->addPosition(QVector3D(minX, minY, 0));
                section->addPosition(QVector3D(maxX, minY, 0));
                section->addPosition(QVector3D(minX + (triangleDelta/2.0f), maxY, 0));

                const float red = static_cast<float>(x)/static_cast<float>(dim);
                const float green = static_cast<float>(y)/static_cast<float>(dim);
                const float colDelta = 0.5f * triangleDelta;

                section->addColor(QColor::fromRgbF(red, green, 0));
                section->addColor(QColor::fromRgbF(red + colDelta, green, 0));
                section->addColor(QColor::fromRgbF(red + (colDelta/2.0f), green + colDelta, 0));

                section->addIndexTriangle(0, 1, 2);

                if ( builder.sectionCount() >= max )
                {
                    return;
                }
            }
        }
    }
}

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
    MaterialStoreEndpoint::MaterialStoreAccessor materialStore = MaterialStoreEndpoint::materialStore();

    m_nFrameBufferId = frameBufferStore->createFrameBuffer(QSize(128, 128));
    m_nRenderModelId = renderModelStore->createRenderModel();

    GeometryBuilder builder(1, 0, QMatrix4x4());
    builder.setMaterialId(materialStore->materialIdFromPath("_debug/debugminimal"));

    createTriangles(builder, 10);

    RenderSystem::FrameDrawParams drawParams;
    drawParams.setRenderMode(ShaderDefs::BarebonesRenderMode);
    drawParams.setBackgroundColor(QColor::fromRgb(0xFF0000FF));

    renderModelStore->addGeometry(m_nRenderModelId, builder);
    renderModelStore->draw(m_nRenderModelId,
                           m_nFrameBufferId,
                           drawParams);

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
