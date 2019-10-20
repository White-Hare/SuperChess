#ifndef GAME_H
#define GAME_H

#include "Engine/DungeonCrawl.h"
#include "Tiles.h"
#include "Pawns.h"

const unsigned rows = 6, columns = 8;
const unsigned tile_length = 64;

class Game
{
	Vec2D<int> offset;

	Window* window;
	SDL_Renderer* renderer;
	SDL_Event* event;
	SDL_Rect map_rect;
	Camera *camera;

	std::vector < std::vector< std::string >> board;

	Tiles *tiles;
	Pawns *whitePawns;
	Pawns *blackPawns;


	double last_time, delta;


public:
	bool running;

    Game();

	void init();
	void load_assets();

    //in loop
	void clock();
	void controlls();
	void move();
	void render();


	~Game();
};

#endif
