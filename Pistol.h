#ifndef PISTOL_H
#define PISTOL_H

#include <SDL.h>

const int pistol_velocity = 400;

inline void pistol(SDL_Rect* rect, int angle, float delta)
{
    if(angle == RIGHT)
	    rect->x += pistol_velocity * delta;
	if (angle == LEFT)
		rect->x -= pistol_velocity * delta;
	if (angle == UP)
		rect->y -= pistol_velocity * delta;
	if (angle == DOWN)
		rect->y += pistol_velocity * delta;
}



#endif
