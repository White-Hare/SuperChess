#ifndef MULTIPLE_OBJECTS_H
#define MULTIPLE_OBJECTS_H

#include "Object.h"
#include <vector>

class MultipleObjects :
	public Object
{
protected:
	std::vector<SDL_Rect*> rects;

public:
	MultipleObjects(const char* ID, SDL_Rect map_rect);

	void append_rect(int x, int y);
	void append_rect(SDL_Rect* rect);
	void pop_rect();
	virtual void erase_rect(int index);

	//pointer->operator[](index);
	Object* &operator[](int index);

	SDL_Rect* get_rect(int index);
	const unsigned get_size();

	template<typename  T> void move(unsigned index, T velocity_x, T velocity_y, float delta, bool place_int_map = true)
	{
		this->move(index, Vec2D<T>{ velocity_x, velocity_y }, delta, place_in_map);
	}
	template<typename  T> void move(unsigned index, Vec2D<T> velocity, float delta, bool pim = true) {

		rects[index]->x += velocity.x * delta;
		rects[index]->y += velocity.y * delta;


		if (pim)
			place_in_map(rects[index], map_rect);
	}

	void place(unsigned index, int x, int y, bool place_in_map = true);

	std::vector<int> collision_list(SDL_Rect* rect2);
	std::vector<int> collision_list(SDL_Point point);
	std::vector<int> collision_list(Circle* circle);


	void render( SDL_Rect *camera, SDL_Renderer* renderer, SDL_Rect* clip = nullptr) override;
	void render( SDL_Rect *camera, SDL_Renderer* renderer, SDL_RendererFlip flip, double angle, SDL_Point* center, SDL_Rect* clip = nullptr) override;

    virtual ~MultipleObjects();
};



#endif
