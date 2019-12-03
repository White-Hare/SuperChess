#include "Board.h"



Board::Board()
{
	board = nullptr;
	rows = 0;
	columns = 0;
}

Board::Board(unsigned rows, unsigned columns)
{
	generateBoard(rows, columns);
}

void Board::generateBoard(unsigned rows, unsigned columns)
{
	this->rows = rows;
	this->columns = columns;

	board = new std::string[rows * columns];

	for (int y = 0; y < rows; y++)
		for (int x = 0; x < columns; x++)
			board[x + y * columns] = ".";
}


void Board::setCellID(unsigned x, unsigned y, std::string ID)
{
	board[x + y * columns] = ID;
}

std::string Board::getCellID(unsigned x, unsigned y)
{
	return board[x + y * columns];
}

unsigned Board::getColumnSize()
{
	return  this->columns;
}

unsigned Board::getRowSize()
{
	return this->rows;
}

unsigned Board::getSize()
{
	return rows * columns;
}


void Board::showBoardAsText()
{
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < columns; x++) {
			std::cout << this->getCellID(x, y);
			for (int i = 0; i < 3 - this->getCellID(x, y).size(); i++)
				std::cout << ' ';

		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

}


Board::~Board()
{
	delete[] board;
}
