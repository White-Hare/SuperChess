#include "Enemies.h"
#include <iostream>
#include <SDL_image.h>

Enemies::Enemies(const char* ID, SDL_Rect map_rect, int velocity):Character(ID, map_rect)
{
	this->velocity = velocity;
	current_frame_pair = new int[2]{0,0};
	this->map_rect = map_rect;
	this->self_rect = nullptr;
	this->texture_ = nullptr;
}


void Enemies::behaviour(void (function)(SDL_Rect*, int,int, float, std::vector<SDL_Rect*>, SDL_Rect*), std::vector<Object*> objects, std::vector<MultipleObjects*> multiple_objects, std::vector<Guns*> bullets, float delta, SDL_Rect* target)
{
	for (auto l : locations_) {

		std::vector<SDL_Rect*> rects;
		for (auto& m_obj : multiple_objects)
			for (int i : m_obj->collision_list(l.first)) 
				rects.push_back(m_obj->get_rect(i));

			
		
		for (auto& obj : objects)
			if (obj->is_collided(l.first))
				rects.push_back(obj->get_self_rect());


		function(l.first, l.second, this->velocity, delta, rects, target);

		place_in_map(l.first, map_rect);
	}
}

void Enemies::add_enemy(SDL_Rect rect, Direction direction)
{
	locations_.push_back({ new SDL_Rect{rect}, direction });
}

void Enemies::add_enemy(int x, int y, Direction direction)
{
	this->add_enemy({ x, y, this->self_rect->w, this->self_rect->h }, direction);
}

void Enemies::erase_enemy(int index)
{
	locations_.erase(locations_.begin() + index);
}

void Enemies::pop_enemy()
{
	locations_.pop_back();
}

void Enemies::move(unsigned index, int x_axis, int y_axis, float delta, bool pim)
{

	if (x_axis && y_axis) {
		locations_[index].first->x += velocity * x_axis * delta * COS45;
		locations_[index].first->y += velocity * y_axis * delta * SIN45;
	}

	else if (x_axis)
		locations_[index].first->x += velocity * x_axis * delta;

	else if (y_axis)
		locations_[index].first->y += velocity * y_axis* delta;


    if(pim)
	    place_in_map(locations_[index].first, map_rect);
}

void Enemies::place(unsigned index, int x, int y, bool pim)
{
	locations_[index].first->x = x;
	locations_[index].first->y = y;

    if(pim)
    	place_in_map(locations_[index].first, map_rect);
}

SDL_Rect* Enemies::get_rect(unsigned index)
{
	return locations_[index].first;
}

const unsigned Enemies::get_size()
{
	return locations_.size();
}

std::vector<int> Enemies::collision_list(SDL_Rect* rect2)
{
	std::vector<int> collided_objects;

	for (int i = 0; i < locations_.size(); i++)
	{
		if (locations_[i].first == nullptr || rect2 == nullptr)
			continue;

		if (locations_[i].first->x > rect2->x + rect2->w)
			continue;

		if (locations_[i].first->x + locations_[i].first ->w < rect2->x)
			continue;

		if (locations_[i].first->y > rect2->y + rect2->h)
			continue;

		if (locations_[i].first->y + locations_[i].first->h < rect2->y)
			continue;

		collided_objects.push_back(i);
	}
	return collided_objects;
}

std::vector<int> Enemies::collision_list(Circle* circle)
{
	std::vector<int> collided_objects;


	for (int i = 0; i < locations_.size(); i++)
	{
		int closest_x, closest_y;

		//for finding closest x
		if (circle->center_x < locations_[i].first->x)
			closest_x = locations_[i].first->x;
		else if (circle->center_x > locations_[i].first->x + locations_[i].first->w)
			closest_x = locations_[i].first->x + locations_[i].first->w;
		else
			closest_x = circle->center_x;


		//for finding closest y
		if (circle->center_y < locations_[i].first->y)
			closest_y = locations_[i].first->y;
		else if (circle->center_y > locations_[i].first->y + locations_[i].first->h)
			closest_y = locations_[i].first->y + locations_[i].first->h;
		else
			closest_y = circle->center_y;

		const int dx = circle->center_x - closest_x;
		const int dy = circle->center_y - closest_y;


		if (dx*dx + dy * dy < circle->radius*circle->radius)
			collided_objects.push_back(i);
	}

	return collided_objects;

}

std::vector<int> Enemies::collision_list(SDL_Point point)
{
	std::vector<int> collided_objects;

	for (int i = 0; i < locations_.size(); i++)
	{
		if ((point.x <= locations_[i].first->x + locations_[i].first->w && point.x >= locations_[i].first->x) &&
			(point.y <= locations_[i].first->y + locations_[i].first->h && point.y >= locations_[i].first->y)) {

			collided_objects.push_back(i);
		}
	}
	return collided_objects;
}

void Enemies::set_velocity(int velocity)
{
	this->velocity = velocity;
}

void Enemies::render(SDL_Rect* camera, SDL_Renderer* renderer, float delta, bool flip_at_turn)
{
	animation_time += delta;

	for (int i : collision_list(camera)) {
		SDL_RendererFlip flip = SDL_FLIP_NONE;


		if (flip_at_turn) {

			if (locations_[i].second == RIGHT || locations_[i].second == DOWN_RIGHT || locations_[i].second == UP_RIGHT)
				flip = SDL_FLIP_NONE;
			if (locations_[i].second == LEFT || locations_[i].second == UP_LEFT || locations_[i].second == DOWN_LEFT)
				flip = SDL_FLIP_HORIZONTAL;

		}

		SDL_Rect* tmp_rect = new SDL_Rect{ -camera->x + locations_[i].first->x, -camera->y + locations_[i].first->y, locations_[i].first->w, locations_[i].first->h };
		SDL_RenderCopyEx(renderer, this->texture_, &this->frames[current_frame + current_frame_pair[0]], tmp_rect, 0, nullptr, flip);
		delete tmp_rect;

	}


	if (animation_time > animation_frequency) {
		current_frame++;
		current_frame %= current_frame_pair[1] - current_frame_pair[0];
		animation_time = 0;
	}
}

Enemies::~Enemies()
{
    
}
