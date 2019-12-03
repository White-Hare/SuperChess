#ifndef BUTTONS_H
#define BUTTONS_H

#include "../MultipleObjects.h"

class Buttons :
	public Object
{
	bool visible;

public:
	Buttons(const char* ID, SDL_Rect map_rect);


	void press_action(void action_callback(Buttons*, Object*), Object* object, SDL_Point cursor_position);
	void press_action(void action_callback(Buttons*, MultipleObjects*), MultipleObjects* object, SDL_Point cursor_position);

	void set_visibilty(bool visibilty);
	bool get_visibilty();


    void render_if_visible(SDL_Renderer* renderer, SDL_Rect* clip =nullptr);
    void render_if_visible(SDL_Renderer* renderer, SDL_RendererFlip flip, double angle = 0, SDL_Point* center = nullptr, SDL_Rect *clip = nullptr);

	~Buttons();
};



#endif
