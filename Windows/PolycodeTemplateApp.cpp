#include "PolycodeTemplateApp.h"
#include "CalliperApp.h"
#include "CCommandLineArgs.h"

PolycodeTemplateApp::PolycodeTemplateApp(PolycodeView *view)
{
	app = new CalliperApp(CalliperApp::RESOLUTION_X_DEFAULT, CalliperApp::RESOLUTION_Y_DEFAULT, false, 0, 0, CalliperApp::FRAMERATE_DEFAULT, false);
	app->appCore = new Win32Core(view, app->targetXRes(), app->targetYRes(), globalCommandLineArgs->startFullscreen, app->targetVsync(), app->targetAALevel(),
		app->targetAnisotropyLevel(), app->targetFrameRate(), -1, app->targetRetinaSupport());
	app->Initialise();
}
PolycodeTemplateApp::~PolycodeTemplateApp()
{
	delete app;
}

bool PolycodeTemplateApp::Update()
{
	return app->Update();
}