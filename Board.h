#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>

class Board
{
	std::string *board;
	unsigned rows, columns;

  public:



	Board();
	Board(unsigned rows, unsigned columns);

    void generateBoard(unsigned rows, unsigned columns);

	void setCellID(unsigned x, unsigned y, std::string ID = ".");
	std::string getCellID(unsigned x, unsigned y);

	unsigned getRowSize();
	unsigned getColumnSize();
	unsigned getSize();


	void showBoardAsText();


	~Board();
};


#endif
