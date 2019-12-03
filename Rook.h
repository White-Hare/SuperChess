#ifndef ROOK_H
#define ROOK_H


#include "Pawns.h"

class Rook :
	public Pawns
{
public:
	Rook(const char* ID, SDL_Rect map_rect, unsigned rows, unsigned columns, Vec2D<int> offset);

	bool is_possible_path(int row, int column, Board *board) override;

	~Rook();
};


#endif
