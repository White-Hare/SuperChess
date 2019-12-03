#ifndef SAVE_AND_LOAD_H
#define SAVE_AND_LOAD_H

#include <fstream>

#include "Board.h"

struct Rules
{
	bool guns_enabled = false;
};

class SaveAndLoad
{
	std::fstream file;
	char* path;

	std::string get_next_word(std::string str, unsigned *index);

public:
	SaveAndLoad();
	SaveAndLoad(const char* path);



	Board* load(Board *board, Rules* rules, unsigned *rows, unsigned *columns);
	void save(Board *board, Rules* rules);

	void open(const char* path ,std::ios::openmode openmode);
	void close();

	void print(std::string str, unsigned *i);
	void twoD_array(std::string tmp, unsigned *columns, unsigned *rows);

	~SaveAndLoad();
};



#endif
