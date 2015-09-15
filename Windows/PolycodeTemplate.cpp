#include <Polycode.h>
#include "PolycodeTemplateApp.h"
#include "PolycodeView.h"
#include "windows.h"
#include "CCommandLineArgs.h"

using namespace Polycode;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	globalCommandLineArgs = new CCommandLineArgs(String(lpCmdLine));
	PolycodeView *view = new PolycodeView(hInstance, nCmdShow, L"Calliper", true, globalCommandLineArgs->showDebugConsole);
	PolycodeTemplateApp *app = new PolycodeTemplateApp(view);

	MSG Msg;
	do {
		while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	} while(app->Update());
	return Msg.wParam;
}