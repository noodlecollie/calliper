#include "CCommandLineArgs.h"

CCommandLineArgs* globalCommandLineArgs = NULL;

#define FLAG_SHOW_DEBUG_CONSOLE "-console"
#define FLAG_START_FULLSCREEN "-fullscreen"

CCommandLineArgs::CCommandLineArgs(const String &commandString)
{
	showDebugConsole = false;
	startFullscreen = false;

	std::vector<String> list = commandString.split(" ");

	for (int i = 0; i < list.size(); i++)
	{
		String s = list[i];
		if (s == FLAG_SHOW_DEBUG_CONSOLE)
		{
			showDebugConsole = true;
			continue;
		}
		else if (s == FLAG_START_FULLSCREEN)
		{
			startFullscreen = true;
			continue;
		}
	}
}