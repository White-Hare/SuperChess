#ifndef KNIGHT_H
#define KNIGHT_H

#include "Pawns.h"
#include "Board.h"

class Knights :
	public Pawns
{

public:
	Knights(const char* ID, SDL_Rect map_rect, unsigned rows, unsigned columns, Vec2D<int> offset = { 0,0 });

    bool is_possible_path(int row, int column, Board *board) override;


	~Knights();
};


#endif
