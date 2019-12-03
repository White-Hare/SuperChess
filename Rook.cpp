#include "Rook.h"



Rook::Rook(const char* ID, SDL_Rect map_rect, unsigned rows, unsigned columns, Vec2D<int> offset) : Pawns(ID, map_rect, rows, columns, offset)
{
}

bool Rook::is_possible_path(int row, int column, Board* board)
{
	int c_column = get_column(selected) , c_row = get_row(selected);

	if (c_column > column)
		std::swap(c_column, column);
	if (c_row > row)
		std::swap(c_row, row);


    for(int x = c_column + 1 ; x < column ; x++)
        if(board->getCellID(x, c_row) != ".")
            return false;

	for (int y = c_row + 1; y < row ; y++)
		if (board->getCellID(c_column, y) != ".")
			return false;


    return (c_row == row) || (c_column == column);
}



Rook::~Rook()
{
}
