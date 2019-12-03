#include "Tiles.h"



Tiles::Tiles(const char* ID, SDL_Rect map_rect): MultipleObjects(ID, map_rect)
{
	this->rows = 0;
	this->columns = 0;
	this->selected_tiles[0] = UNSELECTED;
	this->selected_tiles[1] = UNSELECTED;

	this->offset = { 0,0 };
}

void Tiles::generateTiles(unsigned tile_length, unsigned rows, unsigned collumns, Vec2D<int> offset)
{
	this->rows = rows;
	this->columns = collumns;
	this->self_rect->h = this->self_rect->w = tile_length;
	this->offset = offset;


	this->scale(tile_length, tile_length);

	for (int i = 0; i < rows* collumns; i++) 
		this->append_rect((i % columns) * tile_length + offset.x, (i / collumns) * tile_length + offset.y);

}

int Tiles::get_row(int index)
{
	if (index >= 0 && index < get_size())
		return (rects[index]->x - offset.x) / rects[index]->w;

	else {
		std::cerr << "Index Not Found\n";
		return -1;
	}
}

int Tiles::get_column(int index)
{
	if (index >= 0 && index < get_size())
		return (rects[index]->y - offset.y) / rects[index]->h;

	else {
		std::cerr << "Index Not Found\n";
		return -1;
	}
}



int* Tiles::get_selected_tile()
{
	return  selected_tiles;
}


void Tiles::select(int x, int y)
{

	if (selected_tiles[0] == UNSELECTED) {
		const std::vector<int> coll_list = collision_list({ x,y });

		if (coll_list.size() > 0)
		    selected_tiles[0] = coll_list[0];
	}

	else{
	    const std::vector<int> coll_list = collision_list({ x,y });

		if (coll_list.size() > 0)
		    selected_tiles[1] = coll_list[0];}
}


void Tiles::unselect()
{
	selected_tiles[0] = UNSELECTED;
	selected_tiles[1] = UNSELECTED;
}


void Tiles::showSelectedTiles(SDL_Renderer* renderer)
{
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

	SDL_SetRenderDrawColor(renderer, 200, 50, 50, 100);

	for (int i = 0; i < 2; i++) {
		if (selected_tiles[i] >= 0) {
			SDL_Rect* current_rect = this->get_rect(selected_tiles[i]);
			SDL_RenderDrawRect(renderer, current_rect);
		}
	}
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}


Tiles::~Tiles()
{
}
