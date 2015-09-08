#include "PolycodeTemplateApp.h"
#include "CalliperApp.h"

PolycodeTemplateApp::PolycodeTemplateApp(PolycodeView *view)
{
	app = new CalliperApp();
	app->appCore = new Win32Core(view, CalliperApp::RESOLUTION_X_DEFAULT, CalliperApp::RESOLUTION_Y_DEFAULT,
		false, false, 0, 0, CalliperApp::FRAMERATE_DEFAULT);
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