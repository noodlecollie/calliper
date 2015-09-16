#include "CCommandLineArgs.h"
#include "Polycode.h"

using namespace Polycode;

CCommandLineArgs* globalCommandLineArgs = NULL;

#define FLAG_SHOW_DEBUG_CONSOLE "-console"
#define FLAG_START_FULLSCREEN "-fullscreen"

void parse(CCommandLineArgs &args, const std::vector<String> &list)
{
	for (int i = 0; i < list.size(); i++)
	{
		String s = list[i];
		if (s == FLAG_SHOW_DEBUG_CONSOLE)
		{
			args.showDebugConsole = true;
			continue;
		}
		else if (s == FLAG_START_FULLSCREEN)
		{
			args.startFullscreen = true;
			continue;
		}
	}
}

void setDefaults(CCommandLineArgs &args)
{
	args.showDebugConsole = false;
	args.startFullscreen = false;
}


CCommandLineArgs::CCommandLineArgs(const String &commandString)
{
	std::vector<String> list = commandString.split(" ");
	parse(*this, list);
}

CCommandLineArgs::CCommandLineArgs(int argc, const char** argv)
{
	std::vector<String> list;
	for (int i = 1; i < argc; i++)
	{
		list.push_back(String(argv[i]));
	}
	parse(*this, list);
}
