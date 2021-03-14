#ifndef GEMWINDOW_H
#define GEMWINDOW_H
#include <math.h>
#include  <vector>
#include "BaseWindow.h"


class GemWindow : public BaseWindow<GemWindow>
{
protected:
	POINT position;
	SIZE size;
	COLORREF color;
public:


	int TIMER_COUNTER;
	const int GEM_TIMER = static_cast<const int>(pow(GetPosition().x , GetPosition().y));


	//these allow access to neighbors of each gem
	GemWindow* TOP;
	GemWindow* LEFT;
	GemWindow* BOTTOM;
	GemWindow* RIGHT;

	


	static BOOL tracking;
	PCWSTR ClassName() const { return L"Gem"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND OnMouseClick();

	


	GemWindow()
	{
		//color = RGB(55, 55, 55);
		TIMER_COUNTER = 0;
	}

	~GemWindow() {};

	//getter and setter
	void SetSize(int x, int y) { size.cx = x; size.cy = y; }
	SIZE GetSize() { return size; }

	void SetPosition(int x, int y) { position.x = x; position.y = y; }
	POINT GetPosition() { return position; }

	void SetColor(COLORREF col) { color = col; }
	COLORREF GetColor() { return color; }



};

#endif