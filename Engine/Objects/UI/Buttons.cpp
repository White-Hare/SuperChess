#include "Buttons.h"



Buttons::Buttons(const char* ID, SDL_Rect map_rect) : Object(ID, map_rect)
{
	this->visible = true;
}

void Buttons::set_visibilty(bool visibilty)
{
	this->visible = visibilty;
}

bool Buttons::get_visibilty()
{
	return visible;
}


void Buttons::press_action(void action_callback(Buttons*, Object*), Object* object, SDL_Point cursor_position)
{
    if(visible && is_collided(cursor_position))
	    action_callback(this, object);
}

void Buttons::press_action(void action_callback(Buttons*, MultipleObjects*), MultipleObjects* object, SDL_Point cursor_position)
{
    if(visible && is_collided(cursor_position))
	    action_callback(this, object);
}


void Buttons::render_if_visible(SDL_Renderer* renderer, SDL_Rect* clip)
{
	if (visible) {
		SDL_Rect* camera = new SDL_Rect{ 0,0,map_rect.w,map_rect.h };
	    this->render(camera, renderer, clip);
		delete camera;
	}
}

void Buttons::render_if_visible(SDL_Renderer* renderer, SDL_RendererFlip flip, double angle, SDL_Point* center, SDL_Rect* clip)
{
	if (visible) {
		SDL_Rect* camera = new SDL_Rect{ 0,0,map_rect.w,map_rect.h };
		this->render(camera, renderer, flip, angle, center, clip);
		delete camera;
	}
}


Buttons::~Buttons()
{
}
