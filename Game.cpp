#include "Game.h"
#include "Pistol.h"

#include <gfx/SDL2_gfxPrimitives.h>

#define WIDTH 600
#define HEIGHT 600

Game::Game()
{
	this->running = true;
}

void Game::init()
{
	DungeonCrawl_INIT();

	window = new Window();
	window->createWindow("SuperChess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	renderer = window->createRenderer();
    event = new SDL_Event();



	map_rect = { 0,0,WIDTH,HEIGHT };
	camera = new Camera(map_rect, WIDTH, HEIGHT);

	this->offset = { (WIDTH - tile_length * rows) / 2, (HEIGHT - tile_length * columns) / 2 };

}

void Game::load_assets()
{

	tiles = new Tiles("tiles", map_rect);
	tiles->load_texture("assets/tile.png", renderer);

	whitePawns = new Pawns("WP", map_rect, rows, columns, offset);
	//whitePawns->load_texture("assets/WhitePawn.png", renderer);
    whitePawns->create_blank_texture(renderer, 64, 64);

    whitePawns->scale(tile_length);

	whitePawns->set_render_target(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

	filledCircleRGBA(renderer, 10, 10, 10, 0, 0, 255, 255);
    

	SDL_SetRenderTarget(renderer, nullptr);


	blackPawns = new Pawns("BP", map_rect, rows, columns, offset);
	blackPawns->load_texture("assets/BlackPawn.png", renderer);
	blackPawns->scale(tile_length);


	tiles->generateTiles(tile_length, rows, columns,offset);



    //change rows and columns
	for (int y = 0; y < rows; y++) {
		board.push_back(std::vector<std::string>(0));
	    for (int x = 0; x < columns; x++)
			board[y].push_back(".");
	}


	for (int i = 0; i < rows*2; i++) {
		whitePawns->add_enemy(*tiles->get_rect(i));
		board[whitePawns->get_row(i)][whitePawns->get_column(i)] = whitePawns->ID;
	}


	for (int i = 0; i < rows*2; i++) {
		blackPawns->add_enemy(*tiles->get_rect(rows*columns - 1 - i));
		board[blackPawns->get_row(i)][blackPawns->get_column(i)] = blackPawns->ID;
	}



	blackPawns->gun = new Guns("BGun", map_rect, blackPawns->get_rect(0));
	blackPawns->gun->load_texture("assets/bullet.png", renderer);
	blackPawns->gun->scale(10.f * (tile_length / 64.f), 4.f * (tile_length / 64.f));

	whitePawns->gun = new Guns("WGun", map_rect, blackPawns->get_rect(0));
	whitePawns->gun->load_texture("assets/bullet.png", renderer);
	whitePawns->gun->scale(10.f * (tile_length /64.f), 4.f * (tile_length / 64.f));


    //start clock after loading

	last_time = SDL_GetTicks();
}

void Game::clock()
{
	const double current = SDL_GetTicks();
	delta = (current - last_time) / 1000.0;
	last_time = current;


	if (delta > 0.05)
		delta = 0.05;

}


void Game::move()
{
	blackPawns->gun->behavior(pistol, delta);
	whitePawns->gun->behavior(pistol, delta);


    for(int i=0; i< blackPawns->get_size(); i++)
        for(auto b : whitePawns->gun->collision_list(blackPawns->get_rect(i)))
        {
			board[blackPawns->get_row(i)][blackPawns->get_column(i)] = ".";

			blackPawns->erase_enemy(i);
			whitePawns->gun->erase_rect(b);
        }


	for (int i = 0; i < whitePawns->get_size(); i++)
		for (auto b : blackPawns->gun->collision_list(whitePawns->get_rect(i)))
		{
			board[whitePawns->get_row(i)][whitePawns->get_column(i)] = ".";

			whitePawns->erase_enemy(i);
			blackPawns->gun->erase_rect(b);
		}
}

void Game::controlls()
{
	while (SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT)
			running = false;

		if (event->type == SDL_KEYDOWN)
			if (event->key.keysym.sym == SDLK_ESCAPE)
				running = false;

		if (event->type == SDL_MOUSEBUTTONDOWN)
		{
			if (event->button.button == SDL_BUTTON_LEFT) {

				tiles->select(event->motion.x, event->motion.y);

				int* selected_tiles = tiles->getSelectedTiles();

				if (selected_tiles[0] != UNSELECTED && selected_tiles[1] == UNSELECTED) {
					whitePawns->select(event->motion.x, event->motion.y);
					blackPawns->select(event->motion.x, event->motion.y);


					if (whitePawns->get_selected() != UNSELECTED)
						whitePawns->gun->change_boss(whitePawns->get_rect(whitePawns->get_selected()));

                    if(blackPawns->get_selected() != UNSELECTED)
					    blackPawns->gun->change_boss(blackPawns->get_rect(blackPawns->get_selected()));


				}

				else if (selected_tiles[1] != UNSELECTED)
				{
					this->board = whitePawns->changePawnTile(tiles->get_rect(tiles->getSelectedTiles()[1]), this->board);
					this->board = blackPawns->changePawnTile(tiles->get_rect(tiles->getSelectedTiles()[1]), this->board);

					blackPawns->check_pieces(this->board);
					whitePawns->check_pieces(this->board);

					tiles->unselect();
					whitePawns->unselect();
					blackPawns->unselect();

					blackPawns->gun->change_boss(nullptr);
					whitePawns->gun->change_boss(nullptr);

				}

			}

			if (event->button.button == SDL_BUTTON_RIGHT) {
				if (blackPawns->get_selected() != UNSELECTED)
    				blackPawns->gun->add_bullet(UP);

				if (whitePawns->get_selected() != UNSELECTED)
    				whitePawns->gun->add_bullet(DOWN);

			}
		}
	}
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 50, 100, 150, 255);
	SDL_RenderClear(renderer);

	tiles->render(&camera->camera_rect, renderer);
	tiles->showSelectedTiles(renderer);

	whitePawns->render(&camera->camera_rect, renderer, 0);
    blackPawns->render(&camera->camera_rect, renderer, 0);

	whitePawns->gun->render(&camera->camera_rect, renderer);
	blackPawns->gun->render(&camera->camera_rect, renderer);

	SDL_RenderPresent(renderer);
}


Game::~Game()
{
	delete tiles;
	delete window;
}
