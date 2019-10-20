#ifndef CAMERA_H
#define  CAMERA_H

#include <SDL.h>
#include "../Objects/Object.h"

class Camera
{
	SDL_Rect map_rect;

public:
	SDL_Rect camera_rect;

	Camera(SDL_Rect map_rect, unsigned SCREEN_WIDTH, unsigned SCREEN_HEIGHT);

	void focus(Object* object);
	void move(int velocity_x, int velocity_y, float delta);
	void place(int x, int y);

	void resize_camera(unsigned SCREEN_WIDTH, unsigned SCREEN_HEIGHT);

	bool is_collided(SDL_Rect* rect);

	~Camera();
};

#endif
