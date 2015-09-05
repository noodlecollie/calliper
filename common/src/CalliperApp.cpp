#include "CalliperApp.h"

using namespace Polycode;

CalliperApp::CalliperApp(Core** core, PolycodeView* view)
{
	PlatformSpecificInitialisation(core, view);
}

CalliperApp::~CalliperApp()
{

}

bool CalliperApp::Update()
{
	return m_pCore->updateAndRender();
}

void CalliperApp::PlatformSpecificInitialisation(Core** core, PolycodeView* view)
{
		// Do different things depending on our platform.
#if PLATFORM == PLATFORM_WINDOWS
	*core = new Win32Core(view, RESOLUTION_X_DEFAULT, RESOLUTION_Y_DEFAULT, false, false, 0, 0, FRAMERATE_DEFAULT);	  
#elif PLATFORM == PLATFORM_MAC
	*core = new CocoaCore(view, RESOLUTION_X_DEFAULT, RESOLUTION_Y_DEFAULT, false, false, 0, 0, FRAMERATE_DEFAULT);
#elif PLATFORM == PLATFORM_UNIX
	*core = new SDLCore(view, RESOLUTION_X_DEFAULT, RESOLUTION_Y_DEFAULT, false, false, 0, 0, FRAMERATE_DEFAULT);
#else	// Fail!
	assert(false);
#endif

	m_pCore = *core;
}

void CalliperApp::InitialiseResources()
{
	// Load default content.
	CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
}