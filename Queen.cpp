#include "Queen.h"



Queen::Queen(const char* ID, SDL_Rect map_rect, unsigned rows, unsigned columns, Vec2D<int> offset): Pawns(ID, map_rect, rows, columns,offset)
{
}


bool Queen::is_possible_path(int row, int column, Board* board)
{

	int c_column = get_column(selected), c_row = get_row(selected);

	bool bp = (c_row == row && c_column != column) || (c_column == column && c_row != row);
    bool br = abs(column - c_column) == abs(row - c_row);

    
	//Rook
	if (br) {

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


			if (board->getCellID(c_column, c_row) != ".") {
				br = false;
				break;
			}
		}
	}



    //Bishop
	if (bp) {
		c_column = get_column(selected), c_row = get_row(selected);


		if (c_column > column)
			std::swap(c_column, column);
		if (c_row > row)
			std::swap(c_row, row);


		for (int x = c_column + 1; x < column; x++)
			if (board->getCellID(x, c_row) != ".") {
				bp = false;
				break;
			}

		for (int y = c_row + 1; y < row; y++)
			if (board->getCellID(c_column, y) != ".") {
				bp = false;
				break;
			}
	}



	return bp || br;
}



Queen::~Queen()
{
}
