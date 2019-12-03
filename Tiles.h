#ifndef TILES_H
#define TILES_H

#include "Engine/DungeonCrawl.h"

#define UNSELECTED -1


class Tiles : public MultipleObjects
{
	unsigned rows, columns;
	int selected_tiles[2];

	Vec2D<int> offset;

public:


	Tiles(const char* ID, SDL_Rect map_rect);

	void generateTiles(unsigned tile_length, unsigned rows, unsigned columns, Vec2D<int> offset = {0,0 });

	void select(int x, int y);
	void unselect();

	int get_row(int index);
	int get_column(int index);

	int* get_selected_tile();

	void showSelectedTiles(SDL_Renderer* renderer);

	~Tiles();
};


#endif
