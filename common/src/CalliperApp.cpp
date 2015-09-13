#include "CalliperApp.h"
#include "CUITestWindow.h"

using namespace Polycode;

CalliperApp* globalApp = NULL;

#define MENU_ACTION_QUIT "quit"
#define MENU_ACTION_FULLSCREEN "fullscreen"

CalliperApp::CalliperApp(int xRes, int yRes, bool vSync, int aaLevel, int anisotropyLevel, int framerate, bool retina) : EventDispatcher()
{
	// AppCore is initialised by the platform-specific code.
	appCore = NULL;

	// These are kept by the app in order to facilitate toggling fullscreen.
	m_iWindowedXRes = xRes;
	m_iWindowedYRes = yRes;
	m_iTargetAALevel = aaLevel;
	m_iTargetAnisotropyLevel = anisotropyLevel;
	m_iTargetFramerate = framerate;
	m_bTargetVsync = vSync;
	m_bTargetRetinaSupport = retina;
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
	//CoreServices::getInstance()->getResourceManager()->addArchive("UIThemes.pak");
	//CoreServices::getInstance()->getConfig()->loadConfig("Polycode", "UIThemes/dark/theme.xml");
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
	// These make sure the labels are aligned correctly.
	SceneLabel::defaultAnchor = Vector3(-1.0, -1.0, 0.0);
	SceneLabel::defaultPositionAtBaseline = true;
	SceneLabel::defaultSnapToPixels = true;
	SceneLabel::createMipmapsForLabels = false;
	
	m_pMenuBar = new UIMenuBar(640, GetGlobalMenu());

	UIMenuBarEntry *fileEntry = m_pMenuBar->addMenuBarEntry("File");
	fileEntry->addItem("Quit", MENU_ACTION_QUIT);

	UIMenuBarEntry *windowEntry = m_pMenuBar->addMenuBarEntry("Window");
	windowEntry->addItem("Fullscreen", MENU_ACTION_FULLSCREEN);
	
	m_pMenuBar->addEventListener(this, UIEvent::OK_EVENT);
	m_pScreen->addChild(m_pMenuBar);

	m_pUITestWindow = new CUITestWindow("UI Elements", 350, 450);
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

void CalliperApp::handleEvent(Event* event)
{
	if (event->getDispatcher() == m_pMenuBar)
	{
		handleMenuBarEvent(event);
		return;
	}
	
	EventDispatcher::handleEvent(event);
}

void CalliperApp::handleMenuBarEvent(Event* event)
{
	String action = m_pMenuBar->getSelectedItem();

	if (action == MENU_ACTION_QUIT)
	{
		appCore->Shutdown();
	}
	else if (action == MENU_ACTION_FULLSCREEN)
	{
		setFullscreen(!appCore->isFullscreen());
	}
}

void CalliperApp::setFullscreen(bool fullscreen)
{
	if (fullscreen == appCore->isFullscreen()) return;

	if (fullscreen)
	{
		appCore->setVideoMode(appCore->getScreenWidth(), appCore->getScreenHeight(), true, targetVsync(), targetAALevel(), targetAnisotropyLevel(), targetRetinaSupport());
	}
	else
	{
		appCore->setVideoMode(targetXRes(), targetYRes(), false, targetVsync(), targetAALevel(), targetAnisotropyLevel(), targetRetinaSupport());
	}
}

int CalliperApp::targetXRes() const
{
	return m_iWindowedXRes;
}

int CalliperApp::targetYRes() const
{
	return m_iWindowedYRes;
}

int CalliperApp::targetAALevel() const
{
	return m_iTargetAALevel;
}

int CalliperApp::targetAnisotropyLevel() const
{
	return m_iTargetAnisotropyLevel;
}

int CalliperApp::targetFrameRate() const
{
	return m_iTargetFramerate;
}

bool CalliperApp::targetRetinaSupport() const
{
	return m_bTargetRetinaSupport;
}

bool CalliperApp::targetVsync() const
{
	return m_bTargetVsync;
}