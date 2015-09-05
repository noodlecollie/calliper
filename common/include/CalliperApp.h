#ifndef CALLIPER_APP_H
#define CALLIPER_APP_H

#include "PolycodeView.h"
#include "Polycode.h"

class CalliperApp
{
public:
	// This constructor caters to differing platforms. Pass in the view and the core pointer that
	// need to be initialised on the given platform.
	CalliperApp(Core** core, Polycode::PolycodeView* view);
	
	// Destructor should be called by deleting the app from the platform-specific destructor.
	~CalliperApp();

	// Should be called directly from the platform-specific Update() function.
	bool Update();

	// View setup defaults.
public:
	static const int RESOLUTION_X_DEFAULT = 640;
	static const int RESOLUTION_Y_DEFAULT = 480;
	static const int FRAMERATE_DEFAULT = 60;

private:
	void PlatformSpecificInitialisation(Core** core, PolycodeView* view);
	void InitialiseResources();

private:
	Core* m_pCore;	// Also cache our core object. This should be alive as long as we are.
};

#endif // CALLIPER_APP_H