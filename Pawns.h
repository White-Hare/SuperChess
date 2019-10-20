#pragma once
#include "Engine/Objects/Enemies.h"
class Pawns :
	public Enemies
{
	unsigned selected;
	unsigned rows, columns;
	Vec2D<int> offset;
public:
	Pawns(const char* ID, SDL_Rect map_rect,unsigned rows, unsigned columns, Vec2D<int> offset = {0,0});

	Guns* gun;

	std::vector < std::vector< std::string >> changePawnTile(SDL_Rect* tile_rect, std::vector < std::vector< std::string >> board);
	void check_pieces(std::vector < std::vector< std::string >> board);

	int get_row(int index);
	int get_column(int index);


	void select(int x, int y);
    int get_selected();
	void unselect();

    

	~Pawns();
};

