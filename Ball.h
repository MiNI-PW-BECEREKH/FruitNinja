#ifndef BALL_H
#define BALL_H
#include <windows.h>


class Ball
{
	
public:
    UINT radius;
    POINT coordinate;
    HRGN region;
    INT dx;
    INT dy;
    Ball(UINT r, POINT p, INT deltax, INT deltay)
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