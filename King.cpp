#include "King.h"



King::King(const char* ID, SDL_Rect map_rect, unsigned rows, unsigned columns, Vec2D<int> offset) : Pawns(ID, map_rect, rows, columns, offset)
{
}

bool King::is_possible_path(int row, int column, Board* board)
{

	return abs(column - get_column(selected)) <= 1 && abs(row - get_row(selected)) <= 1;
}



King::~King()
{
}
