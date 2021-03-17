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

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

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
    RECT rc;
    GetClientRect(Window(), &rc);
    srand(PROGRESS_COUNTER);
    FLOAT dx = rand() % 10 > 5 ? 3 : -3;
    POINT p; p.x = rand() % MeasureSize(Window()).cx + rand()%50; p.y = MeasureSize(Window()).cy + 10;
    if (p.x > MeasureSize(Window()).cx/2)
        dx = -3;
    else dx = 3;
    FLOAT dy = (rand() % 30 + 15)*-1;
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




void MainWindow::UpdateBalls()
{

    RECT rc;
    GetClientRect(Window(), &rc);
    HDC pDC = GetDC(Window());
    HDC memDC = CreateCompatibleDC(pDC);
    HBITMAP memBitmap = CreateCompatibleBitmap(pDC, rc.right - rc.left, rc.bottom - rc.top);//TODO:fix
    SelectObject(memDC, memBitmap);
    FillRect(memDC, &rc, (HBRUSH)WHITE_BRUSH);
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
    auto removed = std::remove_if(Balls.begin(), Balls.end(), [rc](Ball& b) {return ((b.coordinate.y > rc.bottom + 5 && b.falling)) ; });
    Balls.erase(removed, Balls.end());

	if(!UPDATE_BALLS)
    for (auto& x: Balls)
    {
        x.dx = 0;
        x.dy = 0;
    }
	
	for(auto&x : Balls)
	{
        
		
        if (x.coordinate.y < -75 )
        {
            x.dy *= -1;
            x.dy /= 2;
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
    if(!DRAW_KNIFE_TRACE)
    {
        for(int i = 0 ; i <= 5; i++)
        {
            mousePolygon[i] = mousePolygon[6];
	    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
	    HPEN oldpen = (HPEN)SelectObject(memDC, pen);
	    if(Polyline(memDC, mousePolygon, 7))
	    {
	        //OutputDebugString(L"DRAWING LINE\n");
	    }
		//HMMM
	    SelectObject(memDC, oldpen);
	    DeleteObject(pen);
	        
        }
    }
    else
    {
        HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
        HPEN oldpen = (HPEN)SelectObject(memDC, pen);
        if (Polyline(memDC, mousePolygon, 7))
        {
            //OutputDebugString(L"DRAWING LINE\n");
        }
        //HMMM
        SelectObject(memDC, oldpen);
        DeleteObject(pen);
    }


    SetTextColor(memDC, RGB(77, 255, 77));
    HFONT font = CreateFont(28, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH,NULL);
    SelectObject(memDC, font);
    SetBkMode(memDC, TRANSPARENT);
	
    DrawText(memDC, convertUINT2LPCWSTR(SCORE).c_str(), convertUINT2LPCWSTR(SCORE).size(), &rc, DT_RIGHT | DT_TOP);

    //ClearProgressBar();
    DrawProgressBar(&memDC,&pDC);
    if(TIME_UP )
    {
        DRAW_END_SCREEN = FALSE;
        HDC memDC2 = CreateCompatibleDC(memDC);
        HBITMAP membitmap2 = CreateCompatibleBitmap(memDC, rc.right - rc.left, rc.bottom - rc.top);
        SelectObject(memDC2, membitmap2);
        //SetDCBrushColor(memDC2, RGB(77, 255, 77));

        SelectObject(memDC2, GetStockObject(DC_BRUSH));
        SetDCBrushColor(memDC2, RGB(0, 255, 0));
        if (!Rectangle(memDC2, rc.left, rc.top, rc.right, rc.bottom))
            OutputDebugString(L"Coloring Failed");

    	
        BLENDFUNCTION blnfn;
        blnfn.BlendOp = AC_SRC_OVER;
        blnfn.BlendFlags = 0;
        blnfn.SourceConstantAlpha = (255 * 50)/100;
        blnfn.AlphaFormat =  0;
        if (!GdiAlphaBlend(memDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, memDC2, 0, 0, rc.right - rc.left, rc.bottom - rc.top, blnfn))
            OutputDebugString(L"Alpha Blend Failed");

        //BitBlt(pDC, 0, 0, MeasureSize(Window()).cx, MeasureSize(Window()).cx, memDC, 0, 0, SRCCOPY);

        SetTextColor(memDC, RGB(255, 255, 255));
        HFONT font = CreateFont(40, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, NULL);
        SelectObject(memDC, font);
        SetBkMode(memDC, TRANSPARENT);

        std::ostringstream ss;
        ss << "SCORE:" << std::endl
            << SCORE;
        std::string s = ss.str();

        RECT modifiedrc = rc;
        modifiedrc.top = rc.top + rc.bottom / 2 - 50;

        DrawText(memDC, s2ws(s).c_str(), s2ws(s).size(), &modifiedrc, DT_CENTER | DT_TOP);

    	

        BitBlt(pDC, 0, 0, MeasureSize(Window()).cx, MeasureSize(Window()).cx, memDC, 0, 0, SRCCOPY);

        DeleteObject(membitmap2);
        DeleteDC(memDC);

    }
    else
     BitBlt(pDC, 0, 0, MeasureSize(Window()).cx, MeasureSize(Window()).cx, memDC, 0, 0, SRCCOPY);

	
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
    DRAW_KNIFE_TRACE = TRUE;
    SPAWN_BALLS = TRUE;
    UPDATE_BALLS = TRUE;
    BALL_COLLISION = TRUE;
    SCORE = 0;
    SetTimer(Window(), SPAWN_TIMER, 500, 0);
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



void MainWindow::DrawProgressBar(HDC *memDC,HDC *pDC)
{
    RECT rc;
    GetClientRect(Window(), &rc);
    SelectObject(*memDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(*memDC, RGB(77, 255, 77));
    Rectangle(*memDC, rc.left, rc.bottom - 20, PROGRESS_COUNTER * rc.right / 600, rc.bottom);
    SetDCBrushColor(*memDC, RGB(226, 224, 223));
    Rectangle(*memDC, PROGRESS_COUNTER * rc.right / 600, rc.bottom - 20, rc.right, rc.bottom);
    if(PROGRESS_COUNTER == 599)
        //neglect adding points just so close to end;
        DRAW_KNIFE_TRACE = FALSE;

    if (PROGRESS_COUNTER == 600)
    {
        DRAW_END_SCREEN = TRUE;
        TIME_UP = TRUE;
        BALL_COLLISION = FALSE;
        SPAWN_BALLS = FALSE;
        UPDATE_BALLS = FALSE;
        //KillTimer(Window(), PHYSICS_TIMER);
        KillTimer(Window(), SPAWN_TIMER);
    }
	PROGRESS_COUNTER += 1;
}


void MainWindow::DetectSlicing(POINT mousepos)
{
    RECT rc;
    GetClientRect(Window(), &rc);
    //if (mousepos.x < rc.left || mousepos.x > rc.right || mousepos.y > rc.bottom || mousepos.y < rc.top)
    //{
    //    for (int i = 0; i <= 5; i++)
    //        mousePolygon[i] = mousePolygon[6];
    //}
    //else if(!DRAW_KNIFE_TRACE)
    //    for (int i = 0; i <= 5; i++)
    //        mousePolygon[i] = mousePolygon[6];
    //else
	if(DRAW_KNIFE_TRACE)
        AddToMousePolygon(mousepos);

	if(BALL_COLLISION)
	for(auto& x: Balls)
	{
		
		if(sqrt(pow(x.coordinate.x - mousepos.x,2)+pow(x.coordinate.y - mousepos.y,2))<= x.radius)
		{
            SCORE++;
            Ball tmp = x;
            auto removed = std::remove_if(Balls.begin(), Balls.end(), [x](Ball& b) {return x.coordinate.x == b.coordinate.x && x.coordinate.y == b.coordinate.y; });
            Balls.erase(removed, Balls.end());
            Ball b1 = Ball(); Ball b2 = Ball(); Ball b3 = Ball(); Ball b4 = Ball();
			//Initialize radius
            b1.radius = tmp.radius / 2;
            b2.radius = tmp.radius / 2;
            b3.radius = tmp.radius / 2;
            b4.radius = tmp.radius / 2;
			//Find coordinates
            b1.coordinate.x = tmp.coordinate.x - b1.radius/2;
            b1.coordinate.y = tmp.coordinate.y - b1.radius/2;
            b2.coordinate.x = tmp.coordinate.x + b2.radius/2;
            b2.coordinate.y = tmp.coordinate.y - b2.radius/2;
            b3.coordinate.x = tmp.coordinate.x - b3.radius/2;
            b3.coordinate.y = tmp.coordinate.y + b3.radius/2;
            b4.coordinate.x = tmp.coordinate.x + b4.radius/2;
            b4.coordinate.y = tmp.coordinate.y + b4.radius/2;
			//falling/
            b1.falling = b2.falling = b3.falling = b4.falling = tmp.falling;
            b1.color = b2.color = b3.color = b4.color = tmp.color;
            //if mouse is to left of these new particles add 1px velo to right and vice versa
			//maybe give a bit of force from the center of tmp
            srand(mousepos.x + mousepos.y);
            if (mousepos.x < b1.coordinate.x)
                b1.dx = tmp.dx + rand() %5;
            else b1.dx = tmp.dx *-1 - rand() % 5;
            if (mousepos.y < b1.coordinate.y)
                b1.dy = tmp.dy * 0.9 + rand() % 5;
            else b1.dy = -1*tmp.dy * 0.9 - rand() % 5;

            if (mousepos.x < b2.coordinate.x)
                b2.dx = tmp.dx + rand() % 5;
            else b2.dx = tmp.dx * -1 - rand() % 5;
            if (mousepos.y < b2.coordinate.y)
                b2.dy = tmp.dy * 0.9 + rand() % 5;
            else b2.dy = -1 * tmp.dy * 0.9 - rand() % 5;

            if (mousepos.x < b3.coordinate.x)
                b3.dx = tmp.dx + rand() % 5;
            else b3.dx = tmp.dx * -1 - rand() % 5;
            if (mousepos.y < b3.coordinate.y)
                b3.dy = tmp.dy * 0.9 + rand() % 5;
            else b3.dy = -1 * tmp.dy * 0.9 - rand() % 5;

            if (mousepos.x < b4.coordinate.x)
                b4.dx = tmp.dx + rand() % 5;
            else b4.dx = tmp.dx * -1 - rand() % 5;
            if (mousepos.y < b4.coordinate.y)
                b4.dy = tmp.dy * 0.9 + rand() % 5;
            else b4.dy = -1 * tmp.dy * 0.9 - rand() % 5;

			
            if(tmp.radius/2 >= 4)
            {
            Balls.push_back(b1);
            Balls.push_back(b2);
            Balls.push_back(b3);
            Balls.push_back(b4);
	            
            }
	            

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
        //BALL_COLLISION = TRUE;
        if (!TIME_UP)
            BALL_COLLISION = TRUE;
        SetLayeredWindowAttributes(Window(), 0, (255 * 100) / 100, LWA_ALPHA);
        UpdateWindow(Window());
        KillTimer(Window(), 42);
        SetTimer(Window(), 42, 1000, NULL);
        POINT mousepos;
        mousepos.x = GET_X_LPARAM(lParam);
        mousepos.y = GET_Y_LPARAM(lParam);

        

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
				BALL_COLLISION = FALSE;
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
