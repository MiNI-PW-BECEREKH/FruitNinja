#include <iostream>
#include <math.h>
#include "GemWindow.h"
#include "GemWindow.h"

BOOL GemWindow::tracking = false;

//github.com/sbecerek/BeJeweled



LRESULT GemWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int num = 0;
    switch (uMsg)
    {
    case WM_DESTROY:
        DestroyWindow(Window());
        break;

    

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, CreateSolidBrush(color));
        EndPaint(m_hwnd, &ps);
        //OutputDebugString(L"PAINT\n");
    }
    return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}