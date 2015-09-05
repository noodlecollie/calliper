#include "PolycodeView.h"
#include "Polycode.h"

class CalliperApp;

using namespace Polycode;

class PolycodeTemplateApp {
public:
    PolycodeTemplateApp(PolycodeView *view);
    ~PolycodeTemplateApp();
    
    bool Update();
    
private:
    Core *core;
	CalliperApp* app;
};