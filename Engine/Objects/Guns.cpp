#include "Guns.h"
#include <iostream>


Guns::Guns(const char* ID, SDL_Rect map_rect, Object *boss) :MultipleObjects(ID, map_rect)
{
	boss_rect_ = boss->get_self_rect();
	firing_frequency = 0;

}
Guns::Guns(const char* ID, SDL_Rect map_rect, SDL_Rect* boss) : MultipleObjects(ID, map_rect)
{
	boss_rect_ = boss;
	firing_frequency = 0;
}


void Guns::change_boss(Object boss)
{
	boss_rect_ = boss.get_self_rect();
}
void Guns::change_boss(SDL_Rect* boss)
{
	boss_rect_ = boss;
}

void Guns::set_firing_frequency(float frequency)
{
	try
	{
	    this->firing_frequency = frequency;
		if (frequency < 0)
			throw "Frequency Can't be less than 0";
	}
    catch (const char* error)
    {
		std::cout << error << '\n';
    }
}


void Guns::add_bullet()
{
	append_rect(new SDL_Rect{ boss_rect_->x + boss_rect_->w/2, boss_rect_->y + boss_rect_->h/2, self_rect->w, self_rect->h });
	angles_.push_back(RIGHT);
}

const float Guns::get_firing_frequency()
{
    return firing_frequency;
}


void Guns::add_bullet(int angle)
{
	append_rect(new SDL_Rect{ boss_rect_->x + boss_rect_->w/2 - self_rect->w/2, boss_rect_->y + boss_rect_->h/2 - self_rect->h/2, self_rect->w, self_rect->h });
	angles_.push_back(angle);
}

void Guns::erase_rect(int index)
{
	try {
		rects.erase(rects.begin() + index);
		angles_.erase(angles_.begin() + index);
	}
	catch (const std::out_of_range& oor)
	{
		std::cout << "Already Erased\n";
	}
}


void Guns::behavior(void (function)(SDL_Rect*, int, float), float delta)
{

	for (int i = 0; i < rects.size(); i++) {
	    function(rects[i], angles_[i], delta);


		if ((rects[i]->x + rects[i]->w < map_rect.x || rects[i]->x > map_rect.x + map_rect.w) ||
			(rects[i]->y + rects[i]->h < map_rect.y || rects[i]->y > map_rect.y + map_rect.h)) {
			erase_rect(i);
		}
	}
}

void Guns::render(SDL_Rect* camera, SDL_Renderer* renderer, SDL_Rect* clip)
{
	for (auto i : collision_list(camera))
	{

		if (angles_[i] == RIGHT) {
			SDL_Rect* tmp_rect = new SDL_Rect{ -camera->x + rects[i]->x, -camera->y + rects[i]->y, rects[i]->w, rects[i]->h };
			SDL_RenderCopy(renderer, this->texture_, clip, tmp_rect);
			delete tmp_rect;
		}

		else {
			SDL_Rect* tmp_rect = new SDL_Rect{ -camera->x + rects[i]->x, -camera->y + rects[i]->y, rects[i]->w, rects[i]->h };
			SDL_RenderCopyEx(renderer, this->texture_, clip, tmp_rect, angles_[i], new SDL_Point{ self_rect->w / 2, self_rect->h / 2 }, SDL_FLIP_NONE);
			delete tmp_rect;
		}
	}
}


void Guns::render(SDL_Rect* camera, SDL_Renderer* renderer, SDL_RendererFlip flip, double angle, SDL_Point* center, SDL_Rect* clip)
{

	for (auto i : collision_list(camera))
	{
		if (angles_[i] == RIGHT) {
			if (is_collided(camera)) {
				SDL_Rect* tmp_rect = new SDL_Rect{ camera->x + rects[i]->x, camera->y + rects[i]->y, rects[i]->w, rects[i]->h };
				SDL_RenderCopyEx(renderer, this->texture_, clip, tmp_rect, angle, center, flip);
				delete tmp_rect;
			}
		}
		else {
			if (is_collided(camera)) {
				SDL_Rect* tmp_rect = new SDL_Rect{ camera->x + rects[i]->x, camera->y + rects[i]->y, rects[i]->w, rects[i]->h };
				SDL_RenderCopyEx(renderer, this->texture_, clip, tmp_rect, angles_[i], new SDL_Point{ self_rect->w / 2, self_rect->h / 2 }, SDL_FLIP_NONE);
				delete tmp_rect;
			}
		}
	}
}

Guns::~Guns()
{
	delete boss_rect_;
	angles_.clear();
}
