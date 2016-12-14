#ifndef DEMOGLWINDOW_H
#define DEMOGLWINDOW_H

#include "views/mapviewwindow.h"

class DemoGLWindow : public HighLevelConvenience::MapViewWindow
{
    Q_OBJECT
public:
    DemoGLWindow();
    ~DemoGLWindow();

protected:
    virtual void initializeGL() override;
    virtual void initShaders() override;
    virtual void initTextures() override;
    virtual void initMaterials() override;
    virtual void initLocalOpenGlSettings() override;
};

#endif // DEMOGLWINDOW_H
