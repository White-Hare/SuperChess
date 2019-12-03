#ifndef DUNGEONCRAWL_H
#define DUNGEONCRAWL_H





#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer/SDL_mixer.h>
#include <SDL_image.h>



#include "Objects/Object.h"
#include "Objects/MultipleObjects.h"
#include "Objects/Character.h"
#include "Objects/Guns.h"
#include "Objects/Enemies.h"

#include "Objects/UI/Buttons.h"

#include "Physics/Vec2D.h"


#include "Others/Window.h"
#include "Others/Camera.h"
#include "Others/Console.h"






static bool DungeonCrawl_INIT()
{
	//Initiliaze SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		std::cout << "SDL couldn't been initialized. SDL_ERROR::" << SDL_GetError() << '\n';
		return 0;
	}
	if (TTF_Init() == -1)
	{
		std::cout << "TTF couldn't been initialized. TTF_ERROR::" << TTF_GetError() << '\n';
		return  0;
	}

	int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(img_flags) & img_flags))
	{
		std::cout << "IMG_Loader couldn't been initialized. IMG_ERROR::" << IMG_GetError() << '\n';
		return  0;
	}
    return true;
}

#endif
