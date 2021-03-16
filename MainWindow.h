#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <windows.h>
#include <string>
#include <vector>

#include "Ball.h"
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
	BOOL TIME_UP;
	//BOOL Initializing;
	COLORREF backgroundColor;
	UINT PROGRESS_COUNTER;
	std::vector<Ball> Balls ;
	INT SCORE;
	POINT mousePolygon[5];
	BYTE mousePolygonMan[5] = {PT_LINETO,PT_LINETO, PT_LINETO, PT_LINETO, PT_LINETO};

	BOOL LogSettings(LPCWSTR);
	DWORD CheckItem(UINT hItem, HMENU hmenu);
	BOOL ClearBoard();
	BOOL CreateBoard();
	BOOL AdjustWindow();
	PCWSTR ClassName() const { return L"Main Window"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	RECT ProgressBar;
	UINT BOARDSIZE;
	
	
	//constructor
	MainWindow()
	{
		TIMER_COUNTER = 0;
	}

	//getter and setter
	void SetClientSize(SIZE s) { clientSize = s; }
	SIZE GetClientSize() { return clientSize; }

	void ClearProgressBar();
	void DrawProgressBar(HDC *);
	void OnBoardSizeSmall();
	void OnBoardSizeMedium();
	void OnBoardSizeBig();
	void OnNewGame();
	void DetectSlicing(POINT);

	void RandomBallSpawn();

	void UpdateBalls();

	void AddToMousePolygon(POINT p)
	{
		
		mousePolygon[5] = p;
		for(int i=0; i< 4 ; i++)
		{
			mousePolygon[i] = mousePolygon[i + 1];
		}
	}

	POINT* GetMousePolygon()
	{
		return &mousePolygon[0];	
	}
	
};

#endif // !MAINWINDOW_H