#include "PolycodeTemplateApp.h"
#include "CalliperApp.h"

PolycodeTemplateApp::PolycodeTemplateApp(PolycodeView *view)
{
	app = new CalliperApp(&core, view);
}
PolycodeTemplateApp::~PolycodeTemplateApp()
{
	delete app;
}

bool PolycodeTemplateApp::Update()
{
	return app->Update();
}