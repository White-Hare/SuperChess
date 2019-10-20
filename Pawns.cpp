#include "Pawns.h"
#include <iostream>
#include <string>

Pawns::Pawns(const char* ID, SDL_Rect map_rect, unsigned rows, unsigned columns, Vec2D<int> offset) :Enemies(ID, map_rect)
{
	selected = -1;
	this->rows = rows;
	this->columns = columns;
	this->offset = offset;
}

void Pawns::select(int x, int y)
{
	std::vector<int> vec = this->collision_list({ x,y });

	if (vec.size() != 0)
    	selected = vec[0];

}

int Pawns::get_selected()
{
	return selected;
}

int Pawns::get_row(int index)
{
	if (index >= 0 && index < get_size())
		return (locations_[index].first->x - offset.x) / locations_[index].first->w;

	else {
		std::cerr << "Index Not Found\n";
	    return -1;
	}
}

int Pawns::get_column(int index)
{
	if (index >= 0 && index < get_size())
        return (locations_[index].first->y - offset.y) / locations_[index].first->h;

	else {
		std::cerr << "Index Not Found\n";
		return -1;
	}
}


std::vector < std::vector< std::string >> Pawns::changePawnTile(SDL_Rect* tile_rect, std::vector < std::vector< std::string >> board)
{
	if (selected != -1) {
		const int t_row = (tile_rect->x - offset.x) / tile_rect->w, t_column = (tile_rect->y - offset.y) / tile_rect->h;

		if ((abs(get_row(selected) - t_row) <= 1 && abs(get_column(selected) - t_column) <= 1) &&
			abs(get_row(selected) - t_row) != abs(get_column(selected) - t_column)) {
			if (board[t_row][t_column][0] != ID[0]) {
				board[get_row(selected)][get_column(selected)] = ".";

				*locations_[selected].first = { tile_rect->x, tile_rect->y,tile_rect->w,tile_rect->h };



				board[t_row][t_column] = this->ID;
			}
		}
	}
	return board;
}

void Pawns::check_pieces(std::vector<std::vector<std::string>> board)
{
    for(int i=0; i < locations_.size(); i++)
    {
		if (board[get_row(i)][get_column(i)] != this->ID)
			erase_enemy(i);
    }
}


void Pawns::unselect()
{
	selected = -1;
}



Pawns::~Pawns()
{
}
