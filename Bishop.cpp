#include "Bishop.h"



Bishop::Bishop(const char* ID, SDL_Rect map_rect, unsigned rows, unsigned columns, Vec2D<int> offset) :Pawns(ID, map_rect, rows, columns, offset)
{
}


bool Bishop::is_possible_path(int row, int column, Board* board)
{
	int c_column = get_column(selected), c_row = get_row(selected);



	if (abs(column - c_column) != abs(row - c_row))
		return false;


	int dx;
	int dy;

	if (column > c_column)
		dx = 1;
	else 
		dx = -1;


	if (row > c_row)
		dy = 1;
	else 
		dy = -1;
	



	for (int i = c_column; i != column - dx; i += dx)
	{
		c_column += dx;
		c_row += dy;


		if (board->getCellID(c_column, c_row) != ".")
			return false;
	}



    return true;
}



Bishop::~Bishop()
{
}
