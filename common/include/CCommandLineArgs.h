#ifndef CCOMMANDLINEARGS_H
#define CCOMMANDLINEARGS_H

#include "Polycode.h"

using namespace Polycode;

class CCommandLineArgs;
extern CCommandLineArgs* globalCommandLineArgs;

class CCommandLineArgs
{
public:
	CCommandLineArgs(const String &commandString);

	bool startFullscreen;
	bool showDebugConsole;
};

#endif	// CCOMMANDLINEARGS_H