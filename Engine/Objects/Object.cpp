#include "Object.h"
#include <SDL_image.h>
#include <iostream>


Object::Object(const char* ID, SDL_Rect map_rect)
{
	this->self_rect = new SDL_Rect{ 0,0,0,0 };
	this->map_rect = map_rect;
	this->texture_ = nullptr;
	this->ID = ID;
	this->pixels = nullptr;
	this->pitch = 0;
	this->access_ = SDL_TEXTUREACCESS_STATIC;
	this->format_ = new SDL_PixelFormat{ SDL_PIXELFORMAT_RGBA8888 };
}

bool Object::load_texture(const char* path, SDL_Renderer* renderer, SDL_Color* color_key)
{
	SDL_Surface* surface = IMG_Load(path);


	if (surface == nullptr)
    {
		std::cout << "FAILED TO LOAD " << path << '\n';
        return false;
    }
	else {
		self_rect = new SDL_Rect{ 0,0,surface->w, surface->h };

		if (color_key != nullptr)
			SDL_SetColorKey(surface, SDL_TRUE,SDL_MapRGB(surface->format, color_key->r, color_key->g, color_key->b));

	    texture_ = SDL_CreateTextureFromSurface(renderer, surface);
		this->access_ = SDL_TEXTUREACCESS_STATIC;
		this->format_ = surface->format;
        

		SDL_FreeSurface(surface);

        return true;
	}
}

bool Object::load_texture(const char* path, SDL_Renderer* renderer, SDL_TextureAccess access, SDL_Color* color_key)
{
	if (access == SDL_TEXTUREACCESS_STATIC)
		return  this->load_texture(path, renderer, color_key);

	SDL_Surface *surface = IMG_Load(path);

	if (surface == nullptr)
	{
		std::cout << "FAILED TO LOAD " << path << '\n';
		return false;
	}

	surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);

	if (surface == nullptr) {
		printf("Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	if (color_key != nullptr)
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, color_key->r, color_key->g, color_key->b));

	this->access_ = access;
	this->self_rect = new SDL_Rect{ 0,0,surface->w, surface->h };



	this->texture_ = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, access, this->self_rect->w, this->self_rect->h);

	if (texture_ == nullptr)
	{

		std::cout << "Texture cannot been created\n";
		return false;
	}

	SDL_BlendMode mode;
	SDL_GetSurfaceBlendMode(surface, &mode);
	SDL_SetTextureBlendMode(texture_, mode);



    if (access == SDL_TEXTUREACCESS_STREAMING )
    {
    	lock_texture();
		SDL_memcpy(this->pixels, surface->pixels, surface->pitch * self_rect->h);
		unlock_texture();
    }

	else if(access == SDL_TEXTUREACCESS_TARGET)
	{
		SDL_SetRenderTarget(renderer, texture_);
		SDL_RenderCopy(renderer, SDL_CreateTextureFromSurface(renderer, surface) , nullptr, nullptr);
		SDL_SetRenderTarget(renderer,nullptr);
	}


	SDL_FreeSurface(surface);
	surface = nullptr;

	return texture_ != nullptr;
}

bool Object::create_blank_texture(SDL_Renderer* renderer, int width, int height, SDL_TextureAccess access, SDL_Color *color_key)
{
    texture_ = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, access, width, height);
	SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
    if (texture_ == nullptr)
		std::cout << " Failed to create Blank Texture\n";
	else {
		this->access_ = access;
		this->self_rect = new SDL_Rect{ 0,0,width, height };

	}

   
	return  texture_ != nullptr;
}

void Object::assign_texture(SDL_Texture* texture, SDL_Rect* rect, SDL_TextureAccess access, SDL_PixelFormat* format)
{
	this->texture_ = texture;
	this->self_rect = new SDL_Rect{ 0,0,rect->w, rect->h };
	this->access_ = access;
	this->format_ = format;
}


void Object::set_render_target(SDL_Renderer* renderer)
{
	if (this->access_ == SDL_TEXTUREACCESS_TARGET)
		SDL_SetRenderTarget(renderer, this->texture_);
}



void Object::scale(int width)
{

	this->self_rect->h = width * (static_cast<float>(self_rect->h) / static_cast<float>(self_rect->w));
	this->self_rect->w = width;
}

void Object::scale(int width, int height)
{
	this->self_rect->w = width;
	this->self_rect->h = height;
}

void Object::place(int x, int y, bool pim)
{  
	self_rect->x = x;
	self_rect->y = y;

    if(pim)
	    place_in_map(self_rect, map_rect);
}



bool Object::lock_texture()
{
	if (pixels != nullptr) {
		std::cout << "Texture has already been locked.\n";
        return false;
	}
	
    if(SDL_LockTexture(texture_, nullptr, &this->pixels, &pitch) == -1) {
		std::cout << "Unable to lock texture.\n";
		return false;
	}

    return true;
}

bool Object::unlock_texture()
{
    if(pixels == nullptr)
    {
		std::cout << "Texture isn't locked.\n";
        return false;
    }

	SDL_UnlockTexture(texture_);
	pixels = nullptr;

    return true;
}



void Object::set_colorModulation(SDL_Color color)
{
	SDL_SetTextureColorMod(texture_, color.r, color.g, color.b);
}

//SDL_BLENDMODE_BLEND, SDL_BLENDMODE_NONE
void Object::set_blendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(texture_, blending);
}

//set blend mode first
void Object::set_Alpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(texture_, alpha);
}


SDL_Rect* Object::get_self_rect()
{
	return self_rect;
}

void* Object::get_pixels()
{
	return pixels;
}

Uint32 Object::get_pixel32(int x, int y)
{
	Uint32 * pixels = static_cast<Uint32*> (this->pixels);
	return pixels[y*(pitch / sizeof(int)) + x];

}

void Object::copy_pixels(void* pixels)
{
	//printf("pixels: %u\n", static_cast<Uint32*>(this->pixels)[0]);
	if (this->pixels != nullptr)
		SDL_memcpy(this->pixels, pixels, pitch * self_rect->h);
	else
		printf("Texture Is Not Been Locked");
}


bool Object::is_collided(SDL_Rect* rect2)
{
	if (self_rect == nullptr || rect2 == nullptr)
        return false;

    if(self_rect->x >= rect2->x + rect2->w)
        return false;

	if (self_rect->x + self_rect->w <= rect2->x)
		return false;

	if (self_rect->y >= rect2->y + rect2->h)
		return false;

	if (self_rect->y + self_rect->h <= rect2->y)
		return false;

    return true;
}

bool Object::is_collided(Circle* circle)
{
	int closest_x, closest_y;

	//for finding closest x
	if (circle->center_x < self_rect->x)
		closest_x = self_rect->x;
	else if (circle->center_x > self_rect->x + self_rect->w)
		closest_x = self_rect->x + self_rect->w;
	else
		closest_x = circle->center_x;


	//for finding closest y
	if (circle->center_y < self_rect->y)
		closest_y = self_rect->y;
	else if (circle->center_y > self_rect->y + self_rect->h)
		closest_y = self_rect->y + self_rect->h;
	else
		closest_y = circle->center_y;

	const int dx = circle->center_x - closest_x;
	const int dy = circle->center_y - closest_y;


	if (dx*dx + dy * dy <= circle->radius*circle->radius)
		return true;


	return false;
}


void Object::render(SDL_Rect* camera, SDL_Renderer* renderer, SDL_Rect* clip)
{
	if (is_collided(camera)) {
		SDL_Rect* tmp_rect = new SDL_Rect{ -camera->x + self_rect->x, -camera->y + self_rect->y, self_rect->w, self_rect->h };
		SDL_RenderCopy(renderer, this->texture_, clip, tmp_rect);
		delete tmp_rect;
	}
}

void Object::render(SDL_Rect* camera, SDL_Renderer* renderer, SDL_RendererFlip flip, double angle, SDL_Point* center, SDL_Rect* clip)
{
	if (is_collided(camera)) {
		SDL_Rect* tmp_rect = new SDL_Rect{ -camera->x + self_rect->x, -camera->y + self_rect->y, self_rect->w, self_rect->h };
		SDL_RenderCopyEx(renderer, this->texture_, clip, tmp_rect, angle, center, flip);
		delete tmp_rect;
	}
}


Object::~Object()
{
	SDL_DestroyTexture(texture_);
	delete self_rect;
}
