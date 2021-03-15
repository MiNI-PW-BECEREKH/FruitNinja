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

    Ball() {     }
	
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

	BOOL operator==(const Ball b)
	{
        if (b.coordinate.x == coordinate.x
            && b.coordinate.y == coordinate.y
            && b.dx == dx
            && dy == dy
            && b.radius == radius
            && b.falling == falling)
        {
            return TRUE;
        }
        else
            return FALSE;
	}

	
};
#endif BALL_H