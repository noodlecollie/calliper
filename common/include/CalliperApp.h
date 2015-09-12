#ifndef CALLIPER_APP_H
#define CALLIPER_APP_H

#include "Polycode.h"
#include "PolycodeUI.h"

using namespace Polycode;

// This global pointer can be used to access the generic app.
class CalliperApp;
extern CalliperApp* globalApp;

class CUITestWindow;

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
	CalliperApp();
	
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
	
protected:
	void handleEvent(Event* event);

private:
	void InitialiseResources();
	void InitialiseGlobals();
	void InitialiseScreen();
	void InitialiseUI();

private:
	Scene* m_pScreen;		// 2D scene that holds the UI etc.
	UIMenuBar* m_pMenuBar;	// Application menu bar.

	UIGlobalMenu* m_pGlobalMenu;
	UIColorPicker* m_pGlobalColourPicker;

	CUITestWindow* m_pUITestWindow;
};

#endif // CALLIPER_APP_H