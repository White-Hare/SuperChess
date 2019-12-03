#include "Pawns.h"
#include <iostream>
#include <string>

Pawns::Pawns(const char* ID, SDL_Rect map_rect, unsigned rows, unsigned columns, Vec2D<int> offset) :Enemies(ID, map_rect)
{
	selected = UNSELECTED;
	this->rows = rows;
	this->columns = columns;
	this->offset = offset;
	this->placed = false;
	this->moving = false;
	this->target_rect = nullptr;
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

int Pawns::get_column(int index)
{
	if (index >= 0 && index < get_size())
		return (locations_[index].first->x - offset.x) / locations_[index].first->w;

	else {
		std::cerr << "Index Not Found\n";
	    return -1;
	}
}

int Pawns::get_row(int index)
{
	if (index >= 0 && index < get_size())
        return (locations_[index].first->y - offset.y) / locations_[index].first->h;

	else {
		std::cerr << "Index Not Found\n";
		return -1;
	}
}

void Pawns::set_targeted_tile(SDL_Rect* tile_rect, Board *board)
{
	if (selected != UNSELECTED) {
		const int t_column = (tile_rect->x - offset.x) / tile_rect->w, t_row = (tile_rect->y - offset.y) / tile_rect->h;



		if (is_possible_path(t_row, t_column, board)) {
			if (board->getCellID(t_column, t_row)[0] != ID[0]) {
				board->setCellID(get_column(selected), get_row(selected), ".");


				target_rect = new SDL_Rect{*tile_rect};
				moving = true;

				board->setCellID(t_column, t_row, this->ID);
			}
		}
	}
}

SDL_Rect* Pawns::get_targeted_tile()
{
	return target_rect;
}

void Pawns::go_to_targeted_tile(float delta, Board* board)
{


	if (selected == UNSELECTED || target_rect == nullptr) {
		check_erased_pieces(board);
	    return;
	}



	const float dx = target_rect->x - locations_[selected].first->x;
	const float dy = target_rect->y - locations_[selected].first->y;


	if (abs(dx) < 9 && abs(dy) < 9) {
		locations_[selected].first->x = target_rect->x;
		locations_[selected].first->y = target_rect->y;

		moving = false;
		unselect();

		return;
	}



	if (dx == 0)
		locations_[selected].first->y += static_cast<float>(velocity) * delta * (dy / abs(dy));

	else if (dy == 0)
		locations_[selected].first->x += static_cast<float>(velocity) * delta * (dx / abs(dx));

	else {
		locations_[selected].first->x += static_cast<float>(velocity) * delta * cos(atan2(dy, dx));
		locations_[selected].first->y += static_cast<float>(velocity) * delta * sin(atan2(dy, dx));
	}

}

bool Pawns::is_possible_path(int row, int column, Board *board)
{/*               //use it for other projects
	return (abs(get_row(selected) - row) <= 1 && abs(get_column(selected) - column) <= 1) &&
		abs(get_row(selected) - row) != abs(get_column(selected) - column);*/

	bool possible = false;

	if (ID[0] == 'B') {

		if (column == get_column(selected) && row + 1 == get_row(selected) && board->getCellID(column, row) == ".")
			return true;


		if (abs(column - get_column(selected)) == 1 && row + 1 == get_row(selected))
			if (board->getCellID(column, row) != ".")
				return true;


		if (get_row(selected) == rows - 2 && column == get_column(selected) && row + 2 == get_row(selected) &&
			board->getCellID(column, row) == "." && board->getCellID(column, row + 1) == ".")
			return true;


		return false;
	}


	if (ID[0] == 'W') {
		if (column == get_column(selected) && row - 1 == get_row(selected) && board->getCellID(column, row) == ".")
			return true;


		if (abs(column - get_column(selected)) == 1 && row - 1 == get_row(selected))
			if (board->getCellID(column, row) != ".")
				return true;


		if (get_row(selected) == 1 && column == get_column(selected) && row - 2 == get_row(selected) &&
			board->getCellID(column, row) == "." && board->getCellID(column, row - 1) == ".")
			return true;


		return false;
	}
}

void Pawns::promote(unsigned index, Pawns* to_what, Board* board, float delta)
{
	while (moving)
		go_to_targeted_tile(delta, board);


	board->setCellID(get_column(index), get_row(index), to_what->ID);
	to_what->add_enemy(*locations_[index].first);
	erase_enemy(index);
}

void Pawns::check_erased_pieces(Board* board)
{
    for(int i=0; i < locations_.size(); i++)
		if (board->getCellID(get_column(i), get_row(i)) != this->ID) 
			erase_enemy(i);
}

void Pawns::place_pieces(Board* board)
{
	if (!placed) {
		for (int y = 0; y < rows; y++)
			for (int x = 0; x < columns; x++)
				if (board->getCellID(x, y) == this->ID)
					add_enemy(x * self_rect->w + offset.x, y * self_rect->h + offset.y);



		placed = true;
	}

}

void Pawns::unselect()
{
	if (!moving) {
		target_rect = nullptr;
		selected = UNSELECTED;
	}
}

Pawns::~Pawns()
{
	delete gun;
}
