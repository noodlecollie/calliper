#include "viewport.h"
#include <QKeyEvent>
#include <QFocusEvent>
#include "openglrenderer.h"
#include "geometrydata.h"
#include <QOpenGLTexture>
#include "resourcemanager.h"
#include "shaderprogram.h"
#include <QMatrix4x4>
#include "mapscene.h"
#include "shaders.h"
#include "scenecamera.h"
#include <QCursor>
#include "geometryfactory.h"
#include <QWheelEvent>
#include "viewportuseroptions.h"
#include <QPushButton>
#include "simplenumericfont.h"
#include "mapdocument.h"
#include "mapgrid.h"
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QApplication>
#include <QSurface>
#include "mainwindow.h"
#include <QStandardPaths>
#include "application.h"
#include "tools.h"
#include "scenecamera.h"
#include "uiscene.h"

#define VIEWPORT_REPAINT_HACK

#ifdef VIEWPORT_REPAINT_HACK
#define REPAINT() repaintHack()
#else
#define REPAINT() repaint()
#endif

static const QColor NO_CAMERA_COLOUR = QColor::fromRgb(0xff00394d);

Viewport::Viewport(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
    //setUpdateBehavior(QOpenGLWidget::PartialUpdate);

    m_colBackground = Viewport::defaultBackgroundColor();
    m_bBackgroundColorChanged = true;
    m_pCamera = NULL;
    m_pDocument = NULL;
    m_bDrawFocusHighlight = false;
    m_bDrawFPS = false;
    m_iRenderTasks = 0;
    m_pPickedObject = NULL;
    m_iScenePickFlags = 0;
    m_iObjectSelectionMask = SceneObject::AllObjectsMask;
    m_pHighlightOutline = NULL;
    setMouseTracking(true);

    m_pEmptyText = NULL;
    m_pNoCameraText = NULL;

    m_pToggleOptions = new QPushButton(QIcon(QPixmap::fromImage(QImage(":/icons/viewport_options.png"))), QString(), this);
    m_pToggleOptions->resize(18,14);

    m_pUserOptions = new ViewportUserOptions(this);
    m_pUserOptions->move(2,16);
    m_pUserOptions->setVisible(false);

    connect(m_pToggleOptions, &QPushButton::clicked, m_pUserOptions, &ViewportUserOptions::toggleVisibility);
    connect(m_pUserOptions, &ViewportUserOptions::focusHighlightStatusChanged, this, &Viewport::setDrawFocusHighlight);
    connect(m_pUserOptions, &ViewportUserOptions::fpsStatusChanged, this, &Viewport::setDrawFPS);

    m_Timer.connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));
    m_Timer.setInterval(1000/60);
}

Viewport::~Viewport()
{
    makeCurrent();
    delete m_pEmptyText;
    delete m_pHighlightOutline;
    delete m_pNoCameraText;
}

void Viewport::updateBackgroundColor()
{
    if ( m_bBackgroundColorChanged )
    {
        glClearColor(m_colBackground.redF(), m_colBackground.greenF(), m_colBackground.blueF(), m_colBackground.alphaF());
        m_bBackgroundColorChanged = false;
    }
}

void Viewport::initializeGL()
{
    initializeOpenGLFunctions();

    updateBackgroundColor();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &m_iVertexArray);
    glBindVertexArray(m_iVertexArray);

    m_pEmptyText = renderer()->createTextQuad(QSize(256,256),
                                              "No active\ndocument",
                                              QColor::fromRgb(0xffd9d9d9),
                                              QFont("Arial", 25),
                                              Qt::AlignCenter);

    m_pNoCameraText = renderer()->createTextQuad(QSize(256,256),
                                              "No active\ncamera",
                                              QColor::fromRgb(0xffd9d9d9),
                                              QFont("Arial", 25),
                                              Qt::AlignCenter);

    m_pHighlightOutline = GeometryFactory::lineRect(1.0f, QColor::fromRgb(0xffff0000));

    m_TimeElapsed.start();
}

// Don't set any sizes using the width and height here!
// These won't be actual per-pixel dimensions!
void Viewport::resizeGL(int w, int h)
{
    if ( m_pCamera )
    {
        m_pCamera->lens()->setAspectRatio((float)w/(float)h);
    }

    QOpenGLWidget::resizeGL(w,h);
}

void Viewport::paintGL()
{
    if ( m_iRenderTasks != 0 )
    {
        processRenderTasks();
    }

    if ( !m_pDocument )
    {
        drawEmpty();
        return;
    }

    if ( !m_pCamera )
    {
        drawNoActiveCamera();
        return;
    }

    int msec = m_TimeElapsed.restart();
    emit preFrame(msec);

    updateBackgroundColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if ( hasFocus() && m_bDrawFocusHighlight )
        drawHighlight();

    if ( m_bDrawFPS )
        drawFPSText(msec);

    drawScene();
}

void Viewport::drawHighlight()
{
    int index;
    index = resourceManager()->shaderIndex(PerVertexColorShader::staticName());
    Q_ASSERT(index >= 0);
    renderer()->setShaderIndex(index);
    renderer()->begin();
    renderer()->drawQuad(m_pHighlightOutline, size(), QRect(0, 0, width()-1, height()-1), Qt::AlignLeft | Qt::AlignTop);
    renderer()->end();
}

void Viewport::keyPressEvent(QKeyEvent *e)
{
    QOpenGLWidget::keyPressEvent(e);
}

void Viewport::keyReleaseEvent(QKeyEvent *e)
{
    QOpenGLWidget::keyReleaseEvent(e);
}

void Viewport::mousePressEvent(QMouseEvent *e)
{
    QOpenGLWidget::mousePressEvent(e);
}

void Viewport::mouseMoveEvent(QMouseEvent *e)
{
    QOpenGLWidget::mouseMoveEvent(e);
}

void Viewport::mouseReleaseEvent(QMouseEvent *e)
{
    QOpenGLWidget::mouseReleaseEvent(e);
}

void Viewport::focusInEvent(QFocusEvent *e)
{
    Q_ASSERT(!application()->mainWindow()->m_pActiveViewport);
    application()->mainWindow()->m_pActiveViewport = this;

    m_Timer.start();

    QOpenGLWidget::focusInEvent(e);
}
void Viewport::focusOutEvent(QFocusEvent *e)
{
    Q_ASSERT(application()->mainWindow()->m_pActiveViewport == this);
    application()->mainWindow()->m_pActiveViewport = NULL;

    m_Timer.stop();

    QOpenGLWidget::focusOutEvent(e);
}

QColor Viewport::backgroundColor() const
{
    return m_colBackground;
}

void Viewport::setBackgroundColor(const QColor &col)
{
    m_colBackground = col;
    m_colBackground.setAlpha(255);
    m_bBackgroundColorChanged = true;
}

QColor Viewport::defaultBackgroundColor()
{
    return QColor::fromRgb(0xff202525);
}

// TODO: Merge common pieces of these functions?
void Viewport::drawEmpty()
{
    updateBackgroundColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int index = resourceManager()->shaderIndex(UnlitTextureShader::staticName());
    Q_ASSERT(index >= 0);
    renderer()->setShaderIndex(index);

    QSize s = size();
    int shortestWin = qMin(s.width(), s.height());
    int shortestTex = qMin(m_pEmptyText->localTexture()->width(), m_pEmptyText->localTexture()->height());
    int dimension = qMin(shortestTex, shortestWin);
    if ( dimension < 1 ) return;

    int x = s.width()/2;
    int y = s.height()/2;

    renderer()->begin();
    renderer()->drawQuad(m_pEmptyText, s, QRect(x, y, dimension, dimension));
    renderer()->end();
}

void Viewport::drawNoActiveCamera()
{
    glClearColor(NO_CAMERA_COLOUR.redF(), NO_CAMERA_COLOUR.greenF(), NO_CAMERA_COLOUR.blueF(), NO_CAMERA_COLOUR.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int index = resourceManager()->shaderIndex(UnlitTextureShader::staticName());
    Q_ASSERT(index >= 0);
    renderer()->setShaderIndex(index);

    QSize s = size();
    int shortestWin = qMin(s.width(), s.height());
    int shortestTex = qMin(m_pEmptyText->localTexture()->width(), m_pEmptyText->localTexture()->height());
    int dimension = qMin(shortestTex, shortestWin);
    if ( dimension < 1 ) return;

    int x = s.width()/2;
    int y = s.height()/2;

    renderer()->begin();
    renderer()->drawQuad(m_pNoCameraText, s, QRect(x, y, dimension, dimension));
    renderer()->end();
}

SceneCamera* Viewport::camera() const
{
    return m_pCamera;
}

void Viewport::setCamera(SceneCamera *camera)
{
    m_pCamera = camera;
    if ( !m_pCamera )
    {
        setBackgroundColor(defaultBackgroundColor());
        return;
    }

    m_pCamera->lens()->setAspectRatio((float)size().width()/(float)size().height());
}

MapDocument* Viewport::document() const
{
    return m_pDocument;
}

void Viewport::setDocument(MapDocument *doc)
{
    m_pDocument = doc;
}

QPoint Viewport::viewCentre() const
{
    return QPoint(size().width()/2, size().height()/2);
}

bool Viewport::drawFocusHighlight() const
{
    return m_bDrawFocusHighlight;
}

void Viewport::setDrawFocusHighlight(bool enabled)
{
    if ( m_bDrawFocusHighlight == enabled ) return;

    m_bDrawFocusHighlight = enabled;
}

bool Viewport::drawFPS() const
{
    return m_bDrawFPS;
}

void Viewport::setDrawFPS(bool enabled)
{
    if ( m_bDrawFPS == enabled ) return;

    m_bDrawFPS = enabled;
}

void Viewport::wheelEvent(QWheelEvent *e)
{
    QOpenGLWidget::wheelEvent(e);
}

void Viewport::drawFPSText(int msec)
{
    float framesPerSecond = 1000.0f/(float)msec;

    int index;
    index = resourceManager()->shaderIndex(UnlitTextureShader::staticName());
    Q_ASSERT(index >= 0);
    renderer()->setShaderIndex(index);
    renderer()->begin();
    resourceManager()->numericFont()->draw(QString("%0").arg(framesPerSecond).toLatin1(), size(), QSize(16,16), QPoint(20,0));
    renderer()->end();
}

void Viewport::debugSaveCurrentFrame()
{
    QOpenGLFramebufferObjectFormat fboFormat;
    fboFormat.setSamples(0);
    fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    QOpenGLFramebufferObject fbo(sizeInPixels(), fboFormat);
    fbo.bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int index = resourceManager()->shaderIndex(BasicLitTextureShader::staticName());
    Q_ASSERT(index >= 0);
    renderer()->setShaderIndex(index);

    renderer()->begin();
    renderer()->renderScene(m_pDocument->scene(), CameraParams(m_pCamera));
    renderer()->end();

    GLfloat f = -1;
    glReadPixels(0,0,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&f);
    qDebug() << "Depth at (0,0) =" << f;

    fbo.release();
    QImage image = fbo.toImage(true);
    QString path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + QString("/framebuffer.png");
    qDebug() << "Saving framebuffer to:" << path;
    image.save(path);
}

void Viewport::saveCurrentFrame()
{
    QOpenGLFramebufferObjectFormat fboFormat;
    fboFormat.setSamples(0);
    fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    QOpenGLFramebufferObject fbo(sizeInPixels(), fboFormat);
    fbo.bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawScene();

    fbo.release();
    QImage image = fbo.toImage(true);
    m_bSaveFrameResult = image.save(m_szSaveFrameFilename);
}

QSize Viewport::sizeInPixels() const
{
    return application()->mainWindow()->devicePixelRatio() * size();
}

void Viewport::drawScene()
{
    int index = resourceManager()->shaderIndex(BasicLitTextureShader::staticName());
    Q_ASSERT(index >= 0);
    renderer()->setShaderIndex(index);

    renderer()->begin();
    renderer()->renderScene(m_pDocument->scene(), CameraParams(m_pCamera));

    glClear(GL_DEPTH_BUFFER_BIT);
    renderer()->renderScene(m_pDocument->uiScene(), CameraParams(m_pCamera->rootToLocal(), m_pCamera->lens()->projectionMatrix()));

    renderer()->end();
}

void Viewport::processRenderTasks()
{
    if ( (m_iRenderTasks & DepthBufferSelect) == DepthBufferSelect )
    {
        selectFromDepthBuffer(m_DepthSelectPos);
        m_iRenderTasks &= ~DepthBufferSelect;
    }

    if ( (m_iRenderTasks & SaveCurrentFrame) == SaveCurrentFrame )
    {
        saveCurrentFrame();
        m_iRenderTasks &= ~SaveCurrentFrame;
    }
}

void Viewport::selectFromDepthBuffer(const QPoint &pos)
{
    QPoint oglPos(pos.x(), size().height() - pos.y() - 1);
    oglPos *= application()->mainWindow()->devicePixelRatio();

    QOpenGLFramebufferObjectFormat fboFormat;
    fboFormat.setSamples(0);
    fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    QOpenGLFramebufferObject fbo(sizeInPixels(), fboFormat);
    fbo.bind();

    m_PickColour = 0xffffffff;
    m_pPickedObject = NULL;

    renderer()->begin();

    if ( (m_iScenePickFlags & BaseScene::MapSceneFlag) == BaseScene::MapSceneFlag )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        QRgb pickCol = 0xffffffff;
        SceneObject* obj = renderer()->selectFromDepthBuffer(m_pDocument->scene(), CameraParams(m_pCamera), oglPos, m_iObjectSelectionMask, &pickCol);
        if ( obj )
        {
            m_PickColour = pickCol;
            m_pPickedObject = obj;
        }
    }

    if ( (m_iScenePickFlags & BaseScene::UISceneFlag) == BaseScene::UISceneFlag )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        QRgb pickCol = 0xffffffff;
        SceneObject* obj = renderer()->selectFromDepthBuffer(m_pDocument->uiScene(), CameraParams(m_pCamera->rootToLocal(), m_pCamera->lens()->projectionMatrix()),
                                                             oglPos, m_iObjectSelectionMask, &pickCol);
        if ( obj )
        {
            m_PickColour = pickCol;
            m_pPickedObject = obj;
        }
    }

    renderer()->end();

    fbo.release();
}

SceneObject* Viewport::pickObjectFromDepthBuffer(int sceneFlags, const QPoint &pos, int selectionMask, QRgb *pickColor)
{
    m_iRenderTasks |= DepthBufferSelect;
    m_DepthSelectPos = pos;
    m_iScenePickFlags = sceneFlags;
    m_iObjectSelectionMask = selectionMask;
    REPAINT();

    if ( pickColor )
        *pickColor = m_PickColour;

    return m_pPickedObject;
}

bool Viewport::saveCurrentFrame(const QString &filename)
{
    m_iRenderTasks |= SaveCurrentFrame;
    m_szSaveFrameFilename = filename;
    repaint();
    return m_bSaveFrameResult;
}

// Due to a bug in Qt 5.6, repaint() doesn't
// invoke a synchronous repaint like it should.
// We force one here.
void Viewport::repaintHack()
{
    QPaintEvent e(QRect(0, 0, size().width(), size().height()));
    paintEvent(&e);
}
