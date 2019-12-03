#ifndef GAME_H
#define GAME_H

#include "Engine/DungeonCrawl.h"

#include "Tiles.h"
#include "Board.h"

#include "SaveAndLoad.h"

#include "Pawns.h"
#include "Knights.h"
#include "Rook.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"


const unsigned tile_length = 64;



class Game
{
	Vec2D<int> offset;

	Window* window;
	SDL_Renderer* renderer;
	SDL_Event* event;
	SDL_Rect map_rect;
	Camera *camera;
	double last_time, delta;


    std::string path;
	SaveAndLoad *file;
	unsigned rows, columns;

	Board *board;
	Tiles *tiles;
	Rules *rules;

	std::vector<Pawns*> white_pieces;
	std::vector<Pawns*> black_pieces;


    Pawns *whitePawns;
	Pawns *blackPawns;

	Knights* whiteKnights;
	Knights* blackKnights;

	Rook* whiteRooks;
	Rook* blackRooks;

	Bishop* whiteBishop;
	Bishop* blackBishop;

	King* whiteKing;
	King* blackKing;

	Queen* whiteQueen;
	Queen* blackQueen;


	std::vector<Buttons*> buttons;


	float velocity = 300.f;

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
