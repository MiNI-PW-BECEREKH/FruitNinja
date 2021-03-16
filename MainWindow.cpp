#include <ctime>
#include "MainWindow.h"
#include <Windowsx.h>
#include "App.h"
#include "resource.h"
#include  "Arithmetics.h"
#include <sstream>
#include "Ball.h"
#include <vector>
#include <map>

#define ACTIVITY_TIMER 42
#define SPAWN_TIMER 69
#define PROGRESSBAR_TIMER 666
#define PHYSICS_TIMER 420
#define SLICING_TIMER 7

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
            //if (App::GetInstance().Gems[i][j].Create(L"gem", WS_OVERLAPPED |WS_CHILD| SS_LEFT | WS_VISIBLE, WS_EX_LEFT| WS_EX_RIGHTSCROLLBAR| WS_EX_LTRREADING, i * 50   , j * 50, 50, 50, Window(), NULL))
        	
            {
                //pass from main window to Gem object
                //App::GetInstance().Gems[i][j].SetSize(50, 50);
                //App::GetInstance().Gems[i][j].SetPosition(i * 50, j * 50);

            	if((i+j) % 2 == 0)
            	{
					HDC hdc = GetDC(Window());
					HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));
                    //App::GetInstance().Gems[i][j].SetColor(RGB(0, 0, 0));
                   //HBRUSH hOldBrush = (HBRUSH)SetClassLongPtr(App::GetInstance().Gems[i][j].Window(), GCLP_HBRBACKGROUND, (LONG_PTR)hbrush);
                    HBRUSH hOldBrush = (HBRUSH)SetClassLongPtr(Window(), GCLP_HBRBACKGROUND, (LONG_PTR)hbrush);
                    SelectObject(hdc, hbrush);
                    Rectangle(hdc, i * 50, j * 50, i * 50 + 50, j * 50 + 50);
                    DeleteObject(hOldBrush);
                    //InvalidateRect(App::GetInstance().Gems[i][j].Window(), NULL, TRUE);
                    ReleaseDC(Window(), hdc);
            	}
                else
                {
                    HDC hdc = GetDC(Window());
					HBRUSH hbrush = CreateSolidBrush(RGB(255, 255, 255));
                    //App::GetInstance().Gems[i][j].SetColor(RGB(255, 255, 255));
                //HBRUSH hOldBrush = (HBRUSH)SetClassLongPtr(App::GetInstance().Gems[i][j].Window(), GCLP_HBRBACKGROUND, (LONG_PTR)hbrush);
                    HBRUSH hOldBrush = (HBRUSH)SetClassLongPtr(Window(), GCLP_HBRBACKGROUND, (LONG_PTR)hbrush);
                    SelectObject(hdc, hbrush);

	                Rectangle(hdc, i * 50, j * 50, i * 50 + 50, j * 50 + 50);
	                
	                DeleteObject(hOldBrush);
	                //InvalidateRect(App::GetInstance().Gems[i][j].Window(), NULL, TRUE);
	                ReleaseDC(Window(), hdc);
                }


            }
            //else
            {
             //   return FALSE;
            }
        }
    }
	
    return TRUE;
}

BOOL MainWindow::ClearBoard()
{
    //for (unsigned int i = 0; i < MAX_GEM_COUNT_X; i++)
    //{
    //    for (unsigned int j = 0; j < MAX_GEM_COUNT_Y; j++)
    //    {
    //        //SendMessage(Gems[i][j].Window(), WM_CLOSE, NULL, NULL);
    //        //Handle WM_DESTROY no need to send message
    //        //below function sends WM_DESTROY
    //        DestroyWindow(App::GetInstance().Gems[i][j].Window());
    //    }
    //}
    HDC hdc = GetDC(Window());
    RECT rc;
    GetClientRect(Window(), &rc);
    FillRect(hdc, &rc, (HBRUSH)COLOR_BACKGROUND);
    ReleaseDC(Window(), hdc);

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

    return InvalidateRect(Window(),NULL,TRUE);
}


void MainWindow::OnBoardSizeSmall()
{
    KillTimer(Window(), ACTIVITY_TIMER);
    cGem.cx = 8; cGem.cy = 6;
    SIZE s; s.cx = 8 * 50; s.cy = 6 * 50 + 20;//last 10 for bar 
    SetClientSize(s);
    AdjustWindow();
    ClearBoard();
    CreateBoard();
    InvalidateRect(Window(), NULL, TRUE);
    //backgroundColor = RGB(42, 69, 31);
    CheckItem(ID_BOARD_SMALL, GetMenu(this->Window()));
    BOARDSIZE = 0;
    //if (LogSettings(L"40015"))
    //    OutputDebugString(L"40015 >> .ini file");
    OnNewGame();
    SetTimer(Window(), ACTIVITY_TIMER, 3000, NULL);
}

void MainWindow::OnBoardSizeMedium()
{
	//be safe about creating the timer twice
    KillTimer(Window(), ACTIVITY_TIMER);
    cGem.cx = 12; cGem.cy = 10;
    SIZE s; s.cx = 12 * 50; s.cy = 10 * 50+20;
    SetClientSize(s);
    AdjustWindow();
    ClearBoard();
    CreateBoard();
    InvalidateRect(Window(), NULL, TRUE);
    //backgroundColor = RGB(42, 69, 31);
    
    CheckItem(ID_BOARD_MEDIUM, GetMenu(this->Window()));
    BOARDSIZE = 1;
    //if (LogSettings(L"40013"))
    //    OutputDebugString(L"40013 >> .ini file");
    OnNewGame();
    SetTimer(Window(), ACTIVITY_TIMER, 3000, NULL);
}

void MainWindow::OnBoardSizeBig()
{
    //be safe about creating the timer twice
    KillTimer(Window(), ACTIVITY_TIMER);
    cGem.cx = 16; cGem.cy = 12;
    SIZE s; s.cx = 16 * 50; s.cy = 12 * 50+20;
    SetClientSize(s);
    AdjustWindow();
    ClearBoard();
    CreateBoard();
    InvalidateRect(Window(), NULL, TRUE);
    //backgroundColor = RGB(42, 69, 31);

    CheckItem(ID_BOARD_BIG, GetMenu(this->Window()));
    BOARDSIZE = 2;
    //if (LogSettings(L"40014"))
    //    OutputDebugString(L"40014 >> .ini file");
    OnNewGame();
    SetTimer(Window(), ACTIVITY_TIMER, 3000, NULL);
}

void MainWindow::RandomBallSpawn()
{
    srand(PROGRESS_COUNTER);
    FLOAT dx = rand() % 10 > 5 ? 3 : -3;
    POINT p; p.x = rand() % MeasureSize(Window()).cx + rand()%50; p.y = MeasureSize(Window()).cy + 10;
    if (p.x > MeasureSize(Window()).cx/2)
        dx = -3;
    else dx = 3;
    FLOAT dy = (rand() % 35 + 15)*-1;
    UINT R = rand() % 255;
    UINT G = rand() % 255;
    UINT B = rand() % 255;
    Ball tmpball =  Ball(75,p,dx,dy);
    //tmpball.region = CreateEllipticRgn(tmpball.coordinate.x, tmpball.coordinate.y, tmpball.coordinate.x + tmpball.radius, tmpball.coordinate.y + tmpball.radius);
    tmpball.falling = FALSE;
    tmpball.color = RGB(R, G, B);
	//if(tmpball.region != NULL)
	Balls.push_back(tmpball);

}

//void DeleteBall(std::vector<Ball> b,Ball bal)
//{
//    for (int i = 0; i < b.size(); i++)
//    {
//        if (b[i].coordinate.x == bal.coordinate.x
//            && b[i].coordinate.y == bal.coordinate.y
//            && b[i].region == bal.region
//            && b[i].dx == bal.dx
//            && b[i].dy == bal.dy)
//        {
//            b.erase(b.begin() + i, b.end() + i + 2);
//        }
//    }
//}



void MainWindow::UpdateBalls()
{

    HDC pDC = GetDC(Window());
    HDC memDC = CreateCompatibleDC(pDC);
    HBITMAP memBitmap = CreateCompatibleBitmap(pDC, MeasureSize(Window()).cx, MeasureSize(Window()).cx);
    SelectObject(memDC, memBitmap);
    RECT rc;
    GetClientRect(Window(), &rc);
    FillRect(memDC, &rc, (HBRUSH)COLOR_BACKGROUND);
    for (unsigned int i = 0; i < cGem.cx; i++)
    {
        for (unsigned int j = 0; j < cGem.cy; j++)
        {

            {

                if ((i + j) % 2 == 0)
                {

                    SelectObject(memDC, GetStockObject(DC_BRUSH));
                    SetDCBrushColor(memDC, RGB(0, 0, 0));
                    Rectangle(memDC, i * 50, j * 50, i * 50 + 50, j * 50 + 50);

                }
                else
                {

                    SelectObject(memDC, GetStockObject(DC_BRUSH));
                    SetDCBrushColor(memDC, RGB(255, 255, 255));
                    Rectangle(memDC, i * 50, j * 50, i * 50 + 50, j * 50 + 50);

                }


            }
        }
    }

    //Get rid ofthe balls that we don't need to care about anymore
    auto removed = std::remove_if(Balls.begin(), Balls.end(), [rc](Ball& b) {return ((b.coordinate.y > rc.bottom + 10 && b.falling)) ; });
    Balls.erase(removed, Balls.end());
	
	for(auto&x : Balls)
	{
        
		
        if (x.coordinate.y < -100 )
        {
            x.dy *= -1;
            x.falling = TRUE;
        }


		//if x's velocities direction to below it means it is falling
        if (x.dy > 0)
            x.falling = TRUE;

        x.dy += 0.98;
        x.coordinate.x += x.dx;
        x.coordinate.y += x.dy;
		

        //Draws once
        SelectObject(memDC, GetStockObject(DC_BRUSH));
        SetDCBrushColor(memDC, x.color);
        
        Ellipse(memDC, x.coordinate.x, x.coordinate.y, x.coordinate.x + x.radius, x.coordinate.y + x.radius);

	}

	//TODO:SCORE text and line is not showing better sleep now
    SelectObject(memDC, GetStockObject(DC_PEN));
    SetDCPenColor(memDC, RGB(255, 255, 0));
    SelectObject(memDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(memDC, RGB(255, 255, 0));
    PolylineTo(memDC,GetMousePolygon(),5);
	
	//HMMM
    SelectObject(memDC, GetStockObject(DC_PEN));
    SetDCPenColor(memDC, RGB(77, 255, 77));
    DrawText(memDC, convertUINT2LPCWSTR(SCORE).c_str(), -1, &rc, DT_TOP | DT_RIGHT);
    //ClearProgressBar();
    DrawProgressBar(&memDC);
    BitBlt(pDC, 0, 0, MeasureSize(Window()).cx, MeasureSize(Window()).cx, memDC, 0, 0,SRCCOPY);
    ReleaseDC(Window(), pDC);
    DeleteDC(memDC);
    DeleteObject(memBitmap);
    //InvalidateRect(Window(), NULL, TRUE);
    //UpdateWindow(Window());
}




void MainWindow::OnNewGame()
{

    Balls.clear();
    TIME_UP = FALSE;
    PROGRESS_COUNTER = 0;
    SCORE = 0;
    SetTimer(Window(), SPAWN_TIMER, 100, 0);
    SetTimer(Window(), PROGRESSBAR_TIMER, 50, 0);
    SetTimer(Window(), PHYSICS_TIMER, 50, 0);
    SetTimer(Window(), SLICING_TIMER, 50, 0);

}

BOOL MainWindow::LogSettings(LPCWSTR str)
{
    if (!WritePrivateProfileString(
        L"GAME",
        L"SIZE",
        str,
        L"./FruitNinja.ini"
    ))
        return FALSE;
    return TRUE;
}



void MainWindow::DrawProgressBar(HDC *memDC)
{
    RECT rc;
    GetClientRect(Window(), &rc);
    SelectObject(*memDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(*memDC, RGB(77, 255, 77));
    Rectangle(*memDC, rc.left, rc.bottom - 20, PROGRESS_COUNTER * rc.right / 600, rc.bottom);
    SetDCBrushColor(*memDC, RGB(226, 224, 223));
    Rectangle(*memDC, PROGRESS_COUNTER * rc.right / 600, rc.bottom - 20, rc.right, rc.bottom);
    if (PROGRESS_COUNTER == 600)
    {
        KillTimer(Window(), PHYSICS_TIMER);
        KillTimer(Window(), SPAWN_TIMER);
    }
	PROGRESS_COUNTER += 1;
}


void MainWindow::DetectSlicing(POINT mousepos)
{
	for(auto& x: Balls)
	{
		
		if(sqrt(pow(x.coordinate.x - mousepos.x,2)+pow(x.coordinate.y - mousepos.y,2))<= x.radius)
		{
            SCORE++;
            Ball tmp = x;

            Ball b1 = Ball(); Ball b2 = Ball(); Ball b3 = Ball(); Ball b4 = Ball();
			//Initialize radius
            b1.radius = tmp.radius / 2;
            b2.radius = tmp.radius / 2;
            b3.radius = tmp.radius / 2;
            b4.radius = tmp.radius / 2;
			//Find coordinates
            b1.coordinate.x = tmp.coordinate.x - b1.radius/3;
            b1.coordinate.y = tmp.coordinate.y - b1.radius/3;
            b2.coordinate.x = tmp.coordinate.x + b2.radius/3;
            b2.coordinate.y = tmp.coordinate.y - b2.radius/3;
            b3.coordinate.x = tmp.coordinate.x - b3.radius/3;
            b3.coordinate.y = tmp.coordinate.y + b3.radius/3;
            b4.coordinate.x = tmp.coordinate.x + b4.radius/3;
            b4.coordinate.y = tmp.coordinate.y + b4.radius/3;
			//falling/
            b1.falling = b2.falling = b3.falling = b4.falling = tmp.falling;
            b1.color = b2.color = b3.color = b4.color = tmp.color;
            //if mouse is to left of these new particles add 1px velo to right and vice versa
			
            b1.dx = b2.dx = b3.dx = b4.dx = tmp.dx;
            b1.dy = b2.dy = b3.dy = b4.dy = tmp.dy*1/2;
            if(tmp.radius/2 >= 10)
            {
            Balls.push_back(b1);
            Balls.push_back(b2);
            Balls.push_back(b3);
            Balls.push_back(b4);
	            
            }
	            
            auto removed = std::remove_if(Balls.begin(), Balls.end(), [x](Ball& b) {return x.coordinate.x == b.coordinate.x && x.coordinate.y == b.coordinate.y; });
            Balls.erase(removed, Balls.end());
		}
	}
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

        SetLayeredWindowAttributes(Window(), 0, (255 * 100) / 100, LWA_ALPHA);
        UpdateWindow(Window());
        KillTimer(Window(), 42);
        SetTimer(Window(), 42, 1000, NULL);
        POINT mousepos;
        mousepos.x = GET_X_LPARAM(lParam);
        mousepos.y = GET_Y_LPARAM(lParam);
        AddToMousePolygon(mousepos);
        //auto removed = std::remove(mousePolygon.begin()+1, mousePolygon.end()+2);
        
		//Here detect collision send the mousepos;
       // DetectSlicing(mousepos);

    }break;

    case WM_NCMOUSEMOVE:
    {

        SetLayeredWindowAttributes(Window(), 0, (255 * 100) / 100, LWA_ALPHA);
        UpdateWindow(Window());
        KillTimer(Window(), 42);
        SetTimer(Window(), 42, 3000, NULL);
    }break;

    case WM_TIMER:
    {
        if (wParam == ACTIVITY_TIMER)
        {
				//OutputDebugString(L"SEMI-TRANSPARENT\n");
                SetWindowLong(Window(), GWL_EXSTYLE, GetWindowLong(Window(), GWL_EXSTYLE) | WS_EX_LAYERED);
                SetLayeredWindowAttributes(Window(), 0, (255 * 50) / 100, LWA_ALPHA);
        		
                UpdateWindow(Window());
        }
    	else if(wParam == SPAWN_TIMER)
    	{
    		//if(!TIME_UP && !Initializing)
				RandomBallSpawn();
    	}
    	else if(wParam == PROGRESSBAR_TIMER)
    	{
            //DrawProgressBar();
    	}
    	else if(wParam == PHYSICS_TIMER)
    	{
    		//if(!TIME_UP && !Initializing)
				UpdateBalls();
    	}
        else if(wParam == SLICING_TIMER)
        {
            POINT mousepos;
            GetCursorPos(&mousepos);
            ScreenToClient(Window(), &mousepos);
            DetectSlicing(mousepos);
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

        
        //OnNewGame();
    	
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
		//..
        EndPaint(m_hwnd, &ps);
    }
    return 0;

    case WM_ERASEBKGND:
        return 1;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}
