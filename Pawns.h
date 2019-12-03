#ifndef PAWNS_H
#define PAWNS_H

#include "Engine/Objects/Enemies.h"
#include "Board.h"


#define UNSELECTED -1

class Pawns :
	public Enemies
{
	unsigned rows, columns;
	Vec2D<int> offset;
	bool placed , moving;

	SDL_Rect* target_rect;


protected:
	unsigned selected;

public:
    

	Pawns(const char* ID, SDL_Rect map_rect,unsigned rows, unsigned columns, Vec2D<int> offset = {0,0});

	Guns* gun;

	void set_targeted_tile(SDL_Rect* tile_rect, Board *board);
	SDL_Rect* get_targeted_tile();
	void go_to_targeted_tile(float delta, Board* board);

	virtual bool is_possible_path(int row, int column, Board *board);

	void place_pieces(Board *board);
	void check_erased_pieces(Board *board);

	void promote(unsigned index, Pawns* to_what, Board* board, float delta);

	int get_row(int index);
	int get_column(int index);


	void select(int x, int y);
    int get_selected();
	void unselect();

    

	~Pawns();
};


#endif
