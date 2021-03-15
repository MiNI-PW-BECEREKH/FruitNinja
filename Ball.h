#ifndef BALL_H
#define BALL_H
#include <windows.h>
#include <vector>



class Ball
{
	
public:
    UINT radius;
    POINT coordinate;
    COLORREF color;
    FLOAT dx;
    FLOAT dy;
    BOOL falling;
    Ball(UINT r, POINT p, FLOAT deltax, FLOAT deltay)
    {
        radius = r;
        coordinate = p;
        dx = deltax;
        dy = deltay;
    }

    ~Ball()
    {

    };

	
	
};
#endif BALL_H