#include "Knights.h"



Knights::Knights(const char* ID, SDL_Rect map_rect, unsigned rows, unsigned columns, Vec2D<int> offset):Pawns(ID, map_rect, rows, columns, offset)
{
}


bool Knights::is_possible_path(int t_row, int t_column, Board *board)
{
	int c_row = get_row(selected), c_column = get_column(selected);

	return (abs(t_row - c_row) == 2 && abs(t_column - c_column) == 1) ||
		(abs(t_row - c_row) == 1 && abs(t_column - c_column) == 2);
}




Knights::~Knights()
{
}
