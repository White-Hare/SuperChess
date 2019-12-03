#ifndef OBJECT_H 
#define OBJECT_H


#include <SDL.h>
#include "../Physics/Vec2D.h"


#define SIN45 0.70710678118
#define COS45 SIN45

static void place_in_map(SDL_Rect* rect, SDL_Rect map_rect)
{
	if (rect->x < map_rect.x)
		rect->x = map_rect.x;
	if (rect->x + rect->w > map_rect.x + map_rect.w)
		rect->x = map_rect.x + map_rect.w - rect->w;

	if (rect->y < map_rect.y)
		rect->y = map_rect.y;
	if (rect->y + rect->h > map_rect.y + map_rect.h)
		rect->y = map_rect.y + map_rect.h - rect->h;
}


struct Circle { int center_x; int center_y; unsigned radius; };



class Object
{
protected:
	SDL_Texture* texture_;
	SDL_Rect *self_rect, map_rect;

	void* pixels;
	int pitch;
	SDL_TextureAccess access_;
	SDL_PixelFormat *format_;

public:
	const char* ID;


	Object(const char* ID, SDL_Rect map_rect);

	bool load_texture(const char* path, SDL_Renderer *renderer,SDL_Color *color_key=nullptr);
    //    SDL_TEXTUREACCESS_STREAMING, SDL_TEXTUREACCESS_STATIC, SDL_TEXTUREACCESS_TARGET
	bool load_texture(const char* path, SDL_Renderer *renderer, SDL_TextureAccess access, SDL_Color *color_key = nullptr);
	bool create_blank_texture(SDL_Renderer* renderer, int width, int height,SDL_TextureAccess access = SDL_TEXTUREACCESS_TARGET, SDL_Color *color_key = nullptr);
	void assign_texture(SDL_Texture* texture, SDL_Rect *rect, SDL_TextureAccess access, SDL_PixelFormat* format);

	//void set_colorkey(SDL_Color colorkey);//FIX IT


    void scale(int width);
	void scale(int width, int height);

	template<typename  T> void move(T velocity_x, T velocity_y, float delta, bool pim = true)
	{
		this->move(Vec2D<T>{ velocity_x, velocity_y }, delta, pim);
	}
	template<typename  T> void move(Vec2D<T> velocity, float delta, bool pim = true)
	{
		self_rect->x += velocity.x * delta;
		self_rect->y += velocity.y * delta;

		if (pim)
			place_in_map(self_rect, map_rect);
	}

    void place(int x, int y, bool place_in_map = true);

	void set_render_target(SDL_Renderer* renderer);
	void set_colorModulation(SDL_Color color);
	void set_blendMode(SDL_BlendMode blending);
	void set_Alpha(Uint8 alpha);

	bool lock_texture();
	bool unlock_texture();

	void* get_pixels();
	void copy_pixels(void* pixels);
	Uint32 get_pixel32(int x, int y);

	SDL_Rect* get_self_rect();
	bool is_collided(SDL_Rect* rect2);
	bool is_collided(Circle* circle);
	bool is_collided(SDL_Point point);


	virtual void render( SDL_Rect *camera, SDL_Renderer* renderer, SDL_Rect *clip = nullptr);
	virtual void render( SDL_Rect *camera,SDL_Renderer* renderer, SDL_RendererFlip flip, double angle = 0, SDL_Point* center = nullptr, SDL_Rect *clip = nullptr);//SDL_FLIP_NONE for default
    
	~Object();
};



#endif
