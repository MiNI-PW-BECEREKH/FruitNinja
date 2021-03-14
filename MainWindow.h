#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <windows.h>
#include <string>
#include <vector>
#include "BaseWindow.h"






class MainWindow : public BaseWindow<MainWindow>
{
protected:
	//size holds the size of the window
	SIZE clientSize;	//size of the window
	


	//vector holding the gems

public:
	SIZE cGem;
	int TIMER_COUNTER;

	static BOOL Initializing;
	COLORREF backgroundColor;
	static BOOL tracking;
	
	
	BOOL LogSettings(LPCWSTR);
	DWORD CheckItem(UINT hItem, HMENU hmenu);
	BOOL ClearBoard();
	BOOL CreateBoard();
	BOOL AdjustWindow();
	PCWSTR ClassName() const { return L"Main Window"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	UINT BOARDSIZE;

	//constructor
	MainWindow()
	{
		TIMER_COUNTER = 0;
	}

	//getter and setter
	void SetClientSize(SIZE s) { clientSize = s; }
	SIZE GetClientSize() { return clientSize; }

	void OnBoardSizeSmall();
	void OnBoardSizeMedium();
	void OnBoardSizeBig();
	void OnNewGame();

	void RandomBallSpawn();

	void DrawBalls();
    


};

#endif // !MAINWINDOW_H