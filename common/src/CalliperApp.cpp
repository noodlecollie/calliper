#include "CalliperApp.h"

using namespace Polycode;

CalliperApp* globalApp = NULL;
UIGlobalMenu* globalMenu = NULL;

CalliperApp::CalliperApp(Core** core, PolycodeView* view)
{
	// Create the core for this platform.
	PlatformSpecificInitialisation(core, view);

	// Load resources that are needed on startup.
	InitialiseResources();

	// Set up the screen scene.
	InitialiseScreen();

	// Set up the menu bar.
	InitialiseMenuBar();
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

	// Set us as the global application.
	// Make sure this hasn't been done yet!
	assert(!globalApp);
	globalApp = this;
}

void CalliperApp::InitialiseResources()
{
	// .pak files are really just renamed ZIPs. They can be opened with the Windows ZIP viewer.
	// addArchive() gets the specified file or path and adds it as a search path. The content itself is not loaded.
	// We do this on the .pak files, but you can also pass an actual folder to this function.
	// addDirResource() then looks at all of the search paths given so far and loads actual resources.

	// Eg. addArchive("default.pak") makes the contents within default.pak visible to the resource manager.
	// default.pak has a root "default" directory which is now visible because of the above call.
	// addDirResource("default") loads all the contents recursively from this folder.

	// Load default content.
	CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);

	// Load HDR shaders etc.
	CoreServices::getInstance()->getResourceManager()->addArchive("hdr.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("hdr");

	// Load UI content.
	CoreServices::getInstance()->getResourceManager()->addArchive("UIThemes.pak");
	CoreServices::getInstance()->getConfig()->loadConfig("Polycode", "UIThemes/default/theme.xml");

	// Load Calliper's native content.
	Logger::log("Adding native content...\n");
	CoreServices::getInstance()->getResourceManager()->addArchive("native.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("native");
}

CalliperApp::SystemIdentifier CalliperApp::GetSystemIdentifier() const
{
#if PLATFORM == PLATFORM_WINDOWS
	return SystemWindows;
#elif PLATFORM == PLATFORM_MAC
	return SystemMac;
#else
	return SystemUnix;
#endif
}

String CalliperApp::GetSystemStringName() const
{
#if PLATFORM == PLATFORM_WINDOWS
	return "Windows";
#elif PLATFORM == PLATFORM_MAC
	return "Mac";
#else
	return "Unix";
#endif
}

Scene* CalliperApp::GetScreen() const
{
	return m_pScreen;
}

void CalliperApp::InitialiseScreen()
{
	// We want 2D top left for standard UI co-ordinates.
	m_pScreen = new Scene(Scene::SCENE_2D_TOPLEFT);

	// Don't do any visibility culling for the screen.
	m_pScreen->doVisibilityChecking(false);
	m_pScreen->getDefaultCamera()->frustumCulling = false;

	// We use nearest neighbour filtering to keep fonts crisp.
	CoreServices::getInstance()->getRenderer()->setTextureFilteringMode(Renderer::TEX_FILTERING_NEAREST);

	// Process input events!
	m_pScreen->rootEntity.processInputEvents = true;
}

void CalliperApp::InitialiseMenuBar()
{
	assert(!globalMenu);

	// Create the global menu.
	// TODO: What exactly is this and why is it needed?
	globalMenu = new UIGlobalMenu();
	UITextInput::setMenuSingleton(globalMenu);
	m_pScreen->addChild(globalMenu);

	// Create the menu bar.
	m_pMenuBar = new UIMenuBar(300, globalMenu);

	UIMenuBarEntry *fileEntry = m_pMenuBar->addMenuBarEntry("File");
	fileEntry->addItem("New File", "new_file", KEY_n);

	m_pScreen->addChild(m_pMenuBar);
}