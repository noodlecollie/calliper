#ifndef CALLIPER_APP_H
#define CALLIPER_APP_H

#include "Polycode.h"
#include "PolycodeUI.h"

using namespace Polycode;

// This global pointer can be used to access the generic app.
class CalliperApp;
extern CalliperApp* globalApp;

class CUITestWindow;
class CVirtualSceneSample;

class CalliperApp : public EventDispatcher
{
public:
	// View setup defaults.
	static const int RESOLUTION_X_DEFAULT = 640;
	static const int RESOLUTION_Y_DEFAULT = 480;
	static const int FRAMERATE_DEFAULT = 60;

	// System identifiers.
	enum SystemIdentifier
	{
		SystemWindows = 0,
		SystemMac,
		SystemUnix
	};

public:
	CalliperApp(int xRes, int yRes, bool vSync, int aaLevel, int anisotropyLevel, int framerate, bool retina);
	
	// Destructor should be called by deleting the app from the platform-specific destructor.
	~CalliperApp();

	// Should be called once the core pointer has been set up via platform-specific code.
	// We used to pass things into the constructor, but this doesn't work on Mac where the
	// code is in Objective-C.
	void Initialise();
	Core* appCore;

	// Should be called directly from the platform-specific Update() function.
	bool Update();

	SystemIdentifier GetSystemIdentifier() const;
	String GetSystemStringName() const;

	Scene* GetScreen() const;
	UIGlobalMenu* GetGlobalMenu() const;
	UIColorPicker* GetGlobalColorPicker() const;

	int targetXRes() const;
	int targetYRes() const;
	bool targetVsync() const;
	int targetAALevel() const;
	int targetAnisotropyLevel() const;
	int targetFrameRate() const;
	bool targetRetinaSupport() const;

	void setFullscreen(bool fullscreen);
	
protected:
	void handleEvent(Event* event);

private:
	void handleMenuBarEvent(Event* event);
	void handleCoreEvent(Event* event);

private:
	void InitialiseResources();
	void InitialiseGlobals();
	void InitialiseScreen();
	void InitialiseUI();
	void updateTargetResFromWindow();

private:
	int m_iWindowedXRes;
	int m_iWindowedYRes;
	int m_iTargetAALevel;
	int m_iTargetAnisotropyLevel;
	int m_iTargetFramerate;
	bool m_bTargetVsync;
	bool m_bTargetRetinaSupport;

	Scene* m_pScreen;		// 2D scene that holds the UI etc.
	UIMenuBar* m_pMenuBar;	// Application menu bar.

	UIGlobalMenu* m_pGlobalMenu;
	UIColorPicker* m_pGlobalColourPicker;
	UIMenuBarEntry* m_pWindowMenu;

	int m_iMenuFullscreenItem;

	CVirtualSceneSample* sample;
};

#endif // CALLIPER_APP_H