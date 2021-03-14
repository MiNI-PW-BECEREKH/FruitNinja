#include "App.h"
#include <windows.h>
#include "MainWindow.h"
#include "resource.h"

//Object oriented template from MC docs.


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{

	MainWindow win;

	//create main window
	if (!win.Create(L"FruitNinja",    WS_SYSMENU|WS_OVERLAPPED| WS_DLGFRAME|  WS_VISIBLE , WS_EX_TOPMOST , CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
	                LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU))))
	{
		return 0;
	}

	App::GetInstance().hCursorSword = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	SetCursor(App::GetInstance().hCursorSword);
	
	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	_ASSERTE(hIcon != 0);
	SendMessage(win.Window(), WM_SETICON, ICON_BIG, (LPARAM)hIcon);


	//load the accelerator - shortcuts
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));
	MSG msg = { };

	// Run the message loop.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//checks for accelerator translation
		//if there is nothing to do from accelerator
		//goes for the normal message loop
		if (!TranslateAccelerator(
			win.Window(),   // handle to receiving window 
			hAccel,         // handle to active accelerator table 
			&msg))          // message data 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		//TranslateMessage(&msg);
		//DispatchMessage(&msg);


	}

	return 0;
}


