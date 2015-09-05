#ifndef CALLIPER_APP_H
#define CALLIPER_APP_H

#include "PolycodeView.h"
#include "Polycode.h"
#include "PolycodeUI.h"

using namespace Polycode;

// This global pointer can be used to access the generic app.
class CalliperApp;
extern CalliperApp* globalApp;

class CalliperApp
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
	// This constructor caters to differing platforms. Pass in the view and the core pointer that
	// need to be initialised on the given platform.
	CalliperApp(Core** core, PolycodeView* view);
	
	// Destructor should be called by deleting the app from the platform-specific destructor.
	~CalliperApp();

	// Should be called directly from the platform-specific Update() function.
	bool Update();

	SystemIdentifier GetSystemIdentifier() const;
	String GetSystemStringName() const;

	Scene* GetScreen() const;

private:
	void PlatformSpecificInitialisation(Core** core, PolycodeView* view);
	void InitialiseResources();
	void InitialiseScreen();
	void InitialiseMenuBar();

private:
	Core* m_pCore;			// Also cache our core object. This should be alive as long as we are.
	Scene* m_pScreen;		// 2D scene that holds the UI etc.
	UIMenuBar* m_pMenuBar;	// Application menu bar.
};

#endif // CALLIPER_APP_H