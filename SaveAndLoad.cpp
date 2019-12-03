#include "SaveAndLoad.h"
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <filesystem>



namespace fs = std::experimental::filesystem;



SaveAndLoad::SaveAndLoad()
{
	this->path = nullptr;
}

SaveAndLoad::SaveAndLoad(const char* path)
{
	open(path, std::ios::in);
}


void SaveAndLoad::open(const char* path, std::ios::openmode openmode)
{
	try {
		file.open(path);
		this->path = (char*)(path);
	}
    catch (std::exception e)
    {
		std::cout << e.what() << '\n';
    }
}

void SaveAndLoad::close()
{
	file.close();
	path = nullptr;
}

Board* SaveAndLoad::load(Board* board, Rules* rules, unsigned *rows, unsigned *columns)
{
	
	if (!file.is_open()) {
		std::cout << "File is not open\n";
		return nullptr;
	}


	std::stringstream ds;

	ds << file.rdbuf();
	std::string str = ds.str();
	std::string tmp;


	unsigned i = 0;
	while (tmp != "end")
	{
		tmp = this->get_next_word(str, &i);

		tmp.erase(std::remove(tmp.begin(), tmp.end(), '\n'), tmp.end());


		if (tmp == "create_board") {
			tmp = this->get_next_word(str, &i);


			twoD_array(tmp, columns, rows);

		    board = new Board(*rows, *columns);
		}

        if(tmp == "guns_enabled")
        {
			tmp = this->get_next_word(str, &i);


			if (tmp == "true")
				rules->guns_enabled = true;

			else if (tmp == "false")
			    rules->guns_enabled = false;
			else
                std::cout<< tmp << " is not recognizable\n";

        }

		if (tmp == "print")
			this->print(str, &i);
		else if (tmp == "println") {
			this->print(str, &i);
			std::cout << std::endl;
		}

		while (str[i++] != ';');
	}



	for (int y = 0; y < *rows; y++)
		for (int x = 0; x < *columns; x++) {

			tmp.clear();
		    tmp = get_next_word(str, &i);


			board->setCellID(x, y, tmp);
		}


	str.clear();
	tmp.clear();

	return board;
}


void SaveAndLoad::save(Board* board, Rules* rules)
{
	if (!file.is_open()) {
		std::cout << "File is not open\n";
	    return;
	}


	fs::resize_file(path, 0);//clear file


	file << "create_board " << board->getColumnSize() << 'X' << board->getRowSize() << ";\n";

	std::string state;
	if (rules->guns_enabled == true)
		state = "true";
	else
		state = "false";

	file << "guns_enabled " << state << ";\n";

	file << "end;\n\n";


	for (int y = 0; y < board->getRowSize(); y++) {
		for (int x = 0; x < board->getColumnSize(); x++)
		{
			std::string tmp = board->getCellID(x, y);

			while (tmp.size() < 3)
				tmp += ' ';
                
			file << tmp;
		}
		file << '\n';
	}
}


std::string SaveAndLoad::get_next_word(std::string str, unsigned* index)
{
	std::string tmp;


	while (str[*index] == ';' || str[*index] == ' ' || str[(*index)] == '\n')  (*index)++;


	do {
		tmp += str[*index];

		if (++(*index) == str.size())
            break;

	} while (str[*index] != ';' && str[*index] != ' ' && str[*index] != '\n');

    
    return tmp;
}

void SaveAndLoad::print(std::string str, unsigned* i)
{
	while (str[(*i)++] != ' ');

    std::string tmp = "";


	while (str[*i] != ';')
		tmp += str[(*i)++];


	std::cout << tmp;
	tmp.clear();
}


void SaveAndLoad::twoD_array(std::string str, unsigned *columns, unsigned *rows)
{
	int index = 0;

	std::string tmp;

	while (str[index] != 'X' && str[index] != 'x')
	{
		tmp += str[index];
		index++;
	}

	index++;
	*columns = std::stoi(tmp);

	tmp.clear();


	while (index < str.size())
	{
		tmp += str[index++];
	}

    *rows = std::stoi(tmp);


	tmp.clear();

}


SaveAndLoad::~SaveAndLoad()
{
	delete &file;
}
