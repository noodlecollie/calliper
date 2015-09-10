#include "CalliperApp.h"
#include "CUITestWindow.h"

using namespace Polycode;

CalliperApp* globalApp = NULL;

CalliperApp::CalliperApp()
{
	// No initialisation here any more - this is platform-specfic.
	appCore = NULL;
}

CalliperApp::~CalliperApp()
{

}

void CalliperApp::Initialise()
{
	assert(appCore);
	globalApp = this;
	
	Logger::log("Working directory: %s\n", appCore->getDefaultWorkingDirectory().c_str());

	// Load resources that are needed on startup.
	InitialiseResources();

	// Initialise all global objects.
	InitialiseGlobals();

	// Set up the UI.
	InitialiseUI();
}

bool CalliperApp::Update()
{
	return appCore->updateAndRender();
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
	Logger::log("Loading default content...\n");
	CoreServices::getInstance()->getResourceManager()->addArchive("default.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("default", false);
	Logger::log("Finished loading default content.\n");

	// Load HDR shaders etc.
	Logger::log("Loading HDR content...\n");
	CoreServices::getInstance()->getResourceManager()->addArchive("hdr.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("hdr");
	Logger::log("Finished loading HDR content.\n");

	// Load UI content.
	Logger::log("Loading standard theme...\n");
	CoreServices::getInstance()->getResourceManager()->addArchive("standardtheme");
	CoreServices::getInstance()->getConfig()->loadConfig("Polycode", "standardtheme/theme.xml");
	Logger::log("Finished loading standard theme.\n");

	// Load Calliper's native content.
	Logger::log("Adding native content...\n");
	CoreServices::getInstance()->getResourceManager()->addArchive("native.pak");
	CoreServices::getInstance()->getResourceManager()->addDirResource("native");
	Logger::log("Finished adding native content.\n");
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

void CalliperApp::InitialiseUI()
{
	m_pUITestWindow = new CUITestWindow("UI Elements", 350, 350);
	m_pScreen->addChild(m_pUITestWindow);
}

void CalliperApp::InitialiseGlobals()
{
	InitialiseScreen();
	
	// Create the global menu.
	// This seems to be a global class which manages pop-up menus.
	// We can use this to make a menu bar at the top of the screen, for example,
	// by creating a pop-up menu when a button is pressed.
	m_pGlobalMenu = new UIGlobalMenu();
	UITextInput::setMenuSingleton(m_pGlobalMenu);
	m_pScreen->addChild(m_pGlobalMenu);

	m_pGlobalColourPicker = new UIColorPicker();
	m_pGlobalColourPicker->setPosition(0, 0);
	m_pScreen->addChild(m_pGlobalColourPicker);
}

UIGlobalMenu* CalliperApp::GetGlobalMenu() const
{
	return m_pGlobalMenu;
}

UIColorPicker* CalliperApp::GetGlobalColorPicker() const
{
	return m_pGlobalColourPicker;
}