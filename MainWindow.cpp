#include <ctime>
#include "MainWindow.h"

#include "App.h"
#include "resource.h"
#include  "Arithmetics.h"
#include <sstream>

//github.com/sbecerek/BeJeweled

std::wstring convertUINT2LPCWSTR(UINT n)
{
    std::wstringstream wss;
    std::wstring str;

    wss << n;
    wss >> str;
    return str.c_str();

}

DWORD MainWindow::CheckItem(UINT hItem, HMENU hmenu)
{
    //First uncheck all
    CheckMenuItem(hmenu, ID_BOARD_SMALL, MF_BYCOMMAND | MF_UNCHECKED);
    CheckMenuItem(hmenu, ID_BOARD_MEDIUM, MF_BYCOMMAND | MF_UNCHECKED);
    CheckMenuItem(hmenu, ID_BOARD_BIG, MF_BYCOMMAND | MF_UNCHECKED);
    //then check the hItem
    return CheckMenuItem(hmenu, hItem, MF_BYCOMMAND | MF_CHECKED);
}

BOOL MainWindow::CreateBoard()
{
    for (unsigned int i = 0; i < cGem.cx; i++)
    {
        for (unsigned int j = 0; j < cGem.cy; j++)
        {
            if (App::GetInstance().Gems[i][j].Create(L"gem", WS_OVERLAPPED |WS_CHILD| SS_LEFT | WS_VISIBLE, WS_EX_LEFT| WS_EX_RIGHTSCROLLBAR| WS_EX_LTRREADING,
                i * 50   ,
                j * 50, 50, 50, Window(), NULL))
            {
                //pass from main window to Gem object
                App::GetInstance().Gems[i][j].SetSize(50, 50);
                App::GetInstance().Gems[i][j].SetPosition(i * 50, j * 50);
                

            	if((i+j) % 2 == 0)
            	{
					HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));
                    App::GetInstance().Gems[i][j].SetColor(RGB(0, 0, 0));
                    HBRUSH hOldBrush = (HBRUSH)SetClassLongPtr(App::GetInstance().Gems[i][j].Window(), GCLP_HBRBACKGROUND, (LONG_PTR)hbrush);
                    DeleteObject(hOldBrush);
                    InvalidateRect(App::GetInstance().Gems[i][j].Window(), NULL, TRUE);
            	}
                else
                {
					HBRUSH hbrush = CreateSolidBrush(RGB(255, 255, 255));
                    App::GetInstance().Gems[i][j].SetColor(RGB(255, 255, 255));
                HBRUSH hOldBrush = (HBRUSH)SetClassLongPtr(App::GetInstance().Gems[i][j].Window(), GCLP_HBRBACKGROUND, (LONG_PTR)hbrush);
                DeleteObject(hOldBrush);
                InvalidateRect(App::GetInstance().Gems[i][j].Window(), NULL, TRUE);
	                
                }


            }
            else
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

BOOL MainWindow::ClearBoard()
{
    for (unsigned int i = 0; i < MAX_GEM_COUNT_X; i++)
    {
        for (unsigned int j = 0; j < MAX_GEM_COUNT_Y; j++)
        {
            //SendMessage(Gems[i][j].Window(), WM_CLOSE, NULL, NULL);
            //Handle WM_DESTROY no need to send message
            //below function sends WM_DESTROY
            DestroyWindow(App::GetInstance().Gems[i][j].Window());
        }
    }

    return UpdateWindow(Window());

}

//github.com/sbecerek/BeJeweled
BOOL MainWindow::AdjustWindow()
{
    //use required fields to set rectangle of client area
    RECT clientToWindowRect;
    clientToWindowRect.left = GetScreenCenter().x - GetClientSize().cx / 2;
    clientToWindowRect.top = GetScreenCenter().y - GetClientSize().cy / 2;
    clientToWindowRect.right = GetScreenCenter().x + GetClientSize().cx / 2;
    clientToWindowRect.bottom = GetScreenCenter().y + GetClientSize().cy / 2;
    //send it to  AdjustWindowRect()
    if (!AdjustWindowRect(&clientToWindowRect,
        WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX |
        WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE,
        TRUE))
    {
        GetLastError();
        return FALSE;
    }
    //use rectangle in MoveWindow()
    if (!MoveWindow(this->Window(),
        CalculateCenter(this->Window(),
            clientToWindowRect).x,
        CalculateCenter(this->Window(),
            clientToWindowRect).y,
        MeasureRect(clientToWindowRect).cx,
        MeasureRect(clientToWindowRect).cy,
        TRUE))
    {
        GetLastError();
        return FALSE;
    }

    return TRUE;
}


void MainWindow::OnBoardSizeSmall()
{
    KillTimer(Window(), 42);
    ClearBoard();
    cGem.cx = 8; cGem.cy = 6;
    SIZE s; s.cx = 8 * 50; s.cy = 6 * 50 + 10;//last 10 for bar 
    SetClientSize(s);
    AdjustWindow();
    CreateBoard();
    InvalidateRect(Window(), NULL, TRUE);
    //backgroundColor = RGB(42, 69, 31);
    CheckItem(ID_BOARD_SMALL, GetMenu(this->Window()));
    BOARDSIZE = 0;
    //if (LogSettings(L"40015"))
    //    OutputDebugString(L"40015 >> .ini file");
    SetTimer(Window(), 42, 3000, NULL);
}

void MainWindow::OnBoardSizeMedium()
{
	//be safe about creating the timer twice
    KillTimer(Window(), 42);
    ClearBoard();
    cGem.cx = 12; cGem.cy = 10;
    SIZE s; s.cx = 12 * 50; s.cy = 10 * 50+10;
    SetClientSize(s);
    AdjustWindow();
    CreateBoard();
    InvalidateRect(Window(), NULL, TRUE);
    //backgroundColor = RGB(42, 69, 31);
    
    CheckItem(ID_BOARD_MEDIUM, GetMenu(this->Window()));
    BOARDSIZE = 1;
    //if (LogSettings(L"40013"))
    //    OutputDebugString(L"40013 >> .ini file");
    SetTimer(Window(), 42, 3000, NULL);
}

void MainWindow::OnBoardSizeBig()
{
    //be safe about creating the timer twice
    KillTimer(Window(), 42);
    ClearBoard();
    cGem.cx = 16; cGem.cy = 12;
    SIZE s; s.cx = 16 * 50; s.cy = 12 * 50+10;
    SetClientSize(s);
    AdjustWindow();
    CreateBoard();
    InvalidateRect(Window(), NULL, TRUE);
    //backgroundColor = RGB(42, 69, 31);

    CheckItem(ID_BOARD_BIG, GetMenu(this->Window()));
    BOARDSIZE = 2;
    //if (LogSettings(L"40014"))
    //    OutputDebugString(L"40014 >> .ini file");
    SetTimer(Window(), 42, 3000, NULL);
}



void MainWindow::OnNewGame()
{
	
}

BOOL MainWindow::LogSettings(LPCWSTR str)
{
  //  // Write data to the file
	 //// For C use LPSTR (char*) or LPWSTR (wchar_t*)
  //  DWORD bytesWritten;
  //  WriteFile(
  //      hFile,            // Handle to the file
  //      str.c_str(),  // Buffer to write
  //      str.size(),   // Buffer size
  //      &bytesWritten,    // Bytes written
  //      nullptr);         // Overlapped
	
    if (!WritePrivateProfileString(
        L"GAME",
        L"SIZE",
        str,
        L"./FruitNinja.ini"
    ))
        return FALSE;
    return TRUE;
}




LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
    {

        switch (LOWORD(wParam))
        {
			case ID_GAME_EXIT:
			{
                DestroyWindow(Window());
			}break;

            case ID_BOARD_SMALL:
            {
                OnBoardSizeSmall();
            }break;

            case ID_BOARD_MEDIUM:
            {
                OnBoardSizeMedium();
            }break;

            case ID_BOARD_BIG:
            {
                OnBoardSizeBig();
            }break;

            case ID_GAME_NEWGAME:
            {
                OnNewGame();
            }break;
        }
    		
    }return 0;

    case WM_MOUSEMOVE:
    {
        OutputDebugString(L"OPAQUE\n");
    	//SetWindowLong(Window(), GWL_EXSTYLE,GetWindowLong(Window(), GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes(Window(), 0, (255 * 100) / 100, LWA_ALPHA);
        UpdateWindow(Window());
        KillTimer(Window(), 42);
        SetTimer(Window(), 42, 1000, NULL);

    }break;

    case WM_NCMOUSEMOVE:
    {
        OutputDebugString(L"OPAQUE\n");
        //SetWindowLong(Window(), GWL_EXSTYLE,GetWindowLong(Window(), GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes(Window(), 0, (255 * 100) / 100, LWA_ALPHA);
        UpdateWindow(Window());
        KillTimer(Window(), 42);
        SetTimer(Window(), 42, 3000, NULL);
    }break;

    case WM_TIMER:
    {
        if (wParam == 42)
        {
				OutputDebugString(L"SEMI-TRANSPARENT\n");
                SetWindowLong(Window(), GWL_EXSTYLE, GetWindowLong(Window(), GWL_EXSTYLE) | WS_EX_LAYERED);
                SetLayeredWindowAttributes(Window(), 0, (255 * 50) / 100, LWA_ALPHA);
                UpdateWindow(Window());
        }
    }break;

    	
    case WM_CREATE:
    {
		//for now let's initialize small
    	//later we will check the config
        //OnBoardSizeSmall();
        UINT profileMessage = GetPrivateProfileInt(L"GAME", L"SIZE", 0, L"./FruitNinja.ini");
    		
		switch (profileMessage)
		{
        case 0:
            OnBoardSizeSmall();
            break;
        case 1:
            OnBoardSizeMedium();
            break;
        case 2:
            OnBoardSizeBig();
            break;
		}
    	
    }return 0;

    case WM_WINDOWPOSCHANGING:
	    {
        WINDOWPOS* pos = (WINDOWPOS*)lParam;
        RECT clientToWindowRect;
        clientToWindowRect.left = GetScreenCenter().x - GetClientSize().cx / 2;
        clientToWindowRect.top = GetScreenCenter().y - GetClientSize().cy / 2;
        clientToWindowRect.right = GetScreenCenter().x + GetClientSize().cx / 2;
        clientToWindowRect.bottom = GetScreenCenter().y + GetClientSize().cy / 2;
        //send it to  AdjustWindowRect()
        AdjustWindowRect(&clientToWindowRect,
            WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX |
            WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE,
            TRUE);
        pos->x = CalculateCenter(Window(),clientToWindowRect).x;
        pos->y = CalculateCenter(Window(), clientToWindowRect).y;
		    
	    }break;


    	
    case WM_SETCURSOR:
        if (LOWORD(lParam) == HTCLIENT)
        {
            SetCursor(App::GetInstance().hCursorSword);
            return TRUE;
        }
        break;

    case WM_DESTROY:
        if (LogSettings(convertUINT2LPCWSTR(BOARDSIZE).c_str()))
        {
            OutputDebugString(convertUINT2LPCWSTR(BOARDSIZE).c_str());
            OutputDebugString(L">> .ini File");
        }
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
        //HBRUSH brush = CreateSolidBrush(backgroundColor);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)COLOR_BACKGROUND);
        //DeleteObject(brush);
        EndPaint(m_hwnd, &ps);
    }
    return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}
