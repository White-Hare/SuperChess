#include "Camera.h"



Camera::Camera(SDL_Rect map_rect, unsigned WIDTH, unsigned HEIGHT)
{
	this->map_rect = map_rect;
	this->camera_rect = { 0,0, static_cast<int>(WIDTH), static_cast<int>(HEIGHT)};
}

void Camera::move(int velocity_x, int velocity_y, float delta)
{
	if (velocity_x && velocity_y) {
		camera_rect.x += velocity_x * delta * COS45;
		camera_rect.y += velocity_y * delta * SIN45;
	}

	else if (velocity_x) {
		camera_rect.x += velocity_x * delta;
	}
	else if (velocity_y)
		camera_rect.y += velocity_y * delta;

	place_in_map(&camera_rect, this->map_rect);
}

void Camera::place(int x, int y)
{

	camera_rect.x = x;
	camera_rect.y = y;

	place_in_map(&camera_rect, map_rect);
}

void Camera::focus(Object* object)
{
	SDL_Rect* rect = object->get_self_rect();

	camera_rect.x = rect->x - camera_rect.w / 2;
	camera_rect.y = rect->y - camera_rect.h / 2;


	place_in_map(&camera_rect, map_rect);
}

void Camera::resize_camera(unsigned SCREEN_WIDTH, unsigned SCREEN_HEIGHT)
{
	this->camera_rect.w = SCREEN_WIDTH;
	this->camera_rect.h = SCREEN_HEIGHT;
}

bool Camera::is_collided(SDL_Rect* rect)
{
	if ((camera_rect.w || camera_rect.h) || rect == nullptr)
		return false;

	if (camera_rect.x > rect->x + rect->w)
		return false;

	if (camera_rect.x + camera_rect.w < rect->x)
		return false;

	if (camera_rect.y > rect->y + rect->h)
		return false;

	if (camera_rect.y + camera_rect.h < rect->y)
		return false;

	return true;
}


Camera::~Camera()
{
}
