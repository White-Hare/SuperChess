#pragma once
#include "Pawns.h"


class Bishop :
	public Pawns
{
public:
	Bishop(const char* ID, SDL_Rect map_rect, unsigned rows, unsigned columns, Vec2D<int> offset = { 0,0 });

	bool is_possible_path(int row, int column, Board* board) override;

	~Bishop();
};

