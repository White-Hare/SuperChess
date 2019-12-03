#include "Game.h"
#include "Pistol.h"


#define WIDTH 600
#define HEIGHT 600

Game::Game()
{
	this->running = true;
	this->rules = new Rules();
}

void Game::init()
{
	DungeonCrawl_INIT();

	window = new Window();
	window->createWindow("SuperChess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	renderer = window->createRenderer();
    event = new SDL_Event();


	this->path = "saves/classic.sc";
	file = new SaveAndLoad(path.c_str());
	board = file->load(board, rules,&rows, &columns);
	file->close();


	map_rect = { 0,0,WIDTH,HEIGHT };
	camera = new Camera(map_rect, WIDTH, HEIGHT);

	this->offset = { static_cast<int>(WIDTH - tile_length * columns) / 2, static_cast<int>(HEIGHT - tile_length * rows) / 2 };

}

void Game::load_assets()
{

	tiles = new Tiles("tiles", map_rect);
	tiles->load_texture("assets/tile.png", renderer);



	whitePawns = new Pawns("WP", map_rect, rows, columns, offset);
	whitePawns->set_velocity(velocity);
    whitePawns->load_texture("assets/WhitePawn.png", renderer);
	whitePawns->scale(tile_length);
	white_pieces.push_back(whitePawns);

	/*	//whitePawns->set_render_target(renderer);
		//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		//SDL_RenderDrawRect(renderer, new const SDL_Rect{ 10,10,54,54 });


		//SDL_SetRenderTarget(renderer, nullptr);*/



	whiteKnights = new Knights("WK", map_rect, rows, columns, offset);
	whiteKnights->set_velocity(velocity);
	whiteKnights->load_texture("assets/WhiteKnight.png", renderer);
	whiteKnights->scale(tile_length);
	white_pieces.push_back(whiteKnights);


	whiteRooks = new Rook("WR", map_rect, rows, columns, offset);
	whiteRooks->set_velocity(velocity);
	whiteRooks->load_texture("assets/WhiteRook.png", renderer);
	whiteRooks->scale(tile_length, tile_length);
	white_pieces.push_back(whiteRooks);


	whiteBishop = new Bishop("WB", map_rect, rows, columns, offset);
	whiteBishop->set_velocity(velocity);
	whiteBishop->load_texture("assets/WhiteBishop.png", renderer);
	whiteBishop->scale(tile_length, tile_length);
	white_pieces.push_back(whiteBishop);


	whiteKing = new King("WM", map_rect, rows, columns, offset);
	whiteKing->set_velocity(velocity);
	whiteKing->load_texture("assets/WhiteKing.png", renderer);
	whiteKing->scale(tile_length, tile_length);
	white_pieces.push_back(whiteKing);


	whiteQueen = new Queen("WQ", map_rect, rows, columns, offset);
	whiteQueen->set_velocity(velocity);
	whiteQueen->load_texture("assets/WhiteQueen.png", renderer);
	whiteQueen->scale(tile_length, tile_length);
	white_pieces.push_back(whiteQueen);





	blackPawns = new Pawns("BP", map_rect, rows, columns, offset);
	blackPawns->set_velocity(velocity);
	blackPawns->load_texture("assets/BlackPawn.png", renderer);
	blackPawns->scale(tile_length);
	black_pieces.push_back(blackPawns);


	blackKnights = new Knights("BK", map_rect, rows, columns, offset);
	blackKnights->set_velocity(velocity);
	blackKnights->load_texture("assets/BlackKnight.png", renderer);
	blackKnights->scale(tile_length);
	black_pieces.push_back(blackKnights);


	blackRooks = new Rook("BR", map_rect, rows, columns, offset);
	blackRooks->set_velocity(velocity);
	blackRooks->load_texture("assets/BlackRook.png", renderer);
	blackRooks->scale(tile_length, tile_length);
	black_pieces.push_back(blackRooks);


	blackBishop = new Bishop("BB", map_rect, rows, columns, offset);
	blackBishop->set_velocity(velocity);
	blackBishop->load_texture("assets/BlackBishop.png", renderer);
	blackBishop->scale(tile_length, tile_length);
	black_pieces.push_back(blackBishop);


	blackKing = new King("BM", map_rect, rows, columns, offset);
	blackKing->set_velocity(velocity);
	blackKing->load_texture("assets/BlackKing.png", renderer);
	blackKing->scale(tile_length, tile_length);
	black_pieces.push_back(blackKing);


	blackQueen = new Queen("BQ", map_rect, rows, columns, offset);
	blackQueen->set_velocity(velocity);
	blackQueen->load_texture("assets/BlackQueen.png", renderer);
	blackQueen->scale(tile_length, tile_length);
	black_pieces.push_back(blackQueen);




	//set Board

	tiles->generateTiles(tile_length, rows, columns, offset);



	for (Pawns* p : white_pieces)
		p->place_pieces(board);

	for (Pawns* p : black_pieces)
		p->place_pieces(board);


    //wepons
	if (rules->guns_enabled) {
		whitePawns->gun = new Guns("WGun", map_rect, static_cast<SDL_Rect*>(nullptr));
		whitePawns->gun->load_texture("assets/bullet.png", renderer);
		whitePawns->gun->scale(10.f * (tile_length / 64.f), 4.f * (tile_length / 64.f));


		blackPawns->gun = new Guns("BGun", map_rect, static_cast<SDL_Rect*>(nullptr));
		blackPawns->gun->load_texture("assets/bullet.png", renderer);
		blackPawns->gun->scale(10.f * (tile_length / 64.f), 4.f * (tile_length / 64.f));
	}



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

	if (rules->guns_enabled) {

		blackPawns->gun->behavior(pistol, delta);
		whitePawns->gun->behavior(pistol, delta);



		for (Pawns* p : black_pieces) {
			for (int i = 0; i < p->get_size(); i++)
				for (auto b : whitePawns->gun->collision_list(p->get_rect(i)))
				{
					board->setCellID(p->get_column(i), p->get_row(i), ".");

					p->erase_enemy(i);
					whitePawns->gun->erase_rect(b);
				}
		}

		for (Pawns* p : white_pieces) {
			for (int i = 0; i < p->get_size(); i++)
				for (auto b : blackPawns->gun->collision_list(p->get_rect(i)))
				{
					board->setCellID(p->get_column(i), p->get_row(i), ".");

					p->erase_enemy(i);
					blackPawns->gun->erase_rect(b);
				}
		}
	}



	for (Pawns *p : white_pieces)
		p->go_to_targeted_tile(delta, board);

	for (Pawns *p : black_pieces)
		p->go_to_targeted_tile(delta, board);



    //improve
	for (int i = 0; i < whitePawns->get_size(); i++)
		if (whitePawns->get_row(i) == rows - 1)
			whitePawns->promote(i, whiteQueen, board, delta);
	

	for (int i = 0; i < blackPawns->get_size(); i++)
		if (blackPawns->get_row(i) == 0)
			blackPawns->promote(i, blackQueen, board, delta);

}

void Game::controlls()
{
	while (SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT) {
			file->open(this->path.c_str(), std::ios_base::out);
			file->save(board, rules);
			file->close();

			running = false;
		}
		if (event->type == SDL_KEYDOWN) {
			if (event->key.keysym.sym == SDLK_ESCAPE) {
				file->open(this->path.c_str(), std::ios_base::out);
				file->save(board, rules);
				file->close();

				running = false;
			}

			if (event->key.keysym.sym == SDLK_SPACE) {
				system("cls");
				board->showBoardAsText();
			}

            if(event->key.keysym.sym == SDLK_s && SDL_GetModState() & KMOD_LCTRL)
            {
				file->open(this->path.c_str(), std::ios_base::out);
				file->save(board, rules);
				file->close();

				std::cout << "Saved\n";
            }

			if (event->key.keysym.sym == SDLK_r && SDL_GetModState() & KMOD_LCTRL)
			{
				white_pieces.clear();
				black_pieces.clear();


				file = new SaveAndLoad(path.c_str());
				board = file->load(board, rules, &rows, &columns);
				file->close();


				this->offset = { static_cast<int>(WIDTH - tile_length * columns) / 2, static_cast<int>(HEIGHT - tile_length * rows) / 2 };

                //improve
			    load_assets();
			}
		}

		if (event->type == SDL_MOUSEBUTTONDOWN)
		{
			if (event->button.button == SDL_BUTTON_LEFT) {

				tiles->select(event->motion.x, event->motion.y);

				int* selected_tiles = tiles->get_selected_tile();

				if (selected_tiles[0] != UNSELECTED && selected_tiles[1] == UNSELECTED) {

					for (Pawns *p : white_pieces) {
						p->unselect();
						p->select(event->motion.x, event->motion.y);
					}
					for (Pawns *p : black_pieces) {
						p->unselect();
						p->select(event->motion.x, event->motion.y);
					}

					if (rules->guns_enabled) {
						if (whitePawns->get_selected() != UNSELECTED)
							whitePawns->gun->change_boss(whitePawns->get_rect(whitePawns->get_selected()));


						if (blackPawns->get_selected() != UNSELECTED)
							blackPawns->gun->change_boss(blackPawns->get_rect(blackPawns->get_selected()));
					}

				}

				else if (selected_tiles[1] != UNSELECTED)
				{

					for (Pawns *p : white_pieces)
						p->set_targeted_tile(tiles->get_rect(tiles->get_selected_tile()[1]), board);
			
					for (Pawns *p : black_pieces)
					    p->set_targeted_tile(tiles->get_rect(tiles->get_selected_tile()[1]), board);



					tiles->unselect();


					for (int i = 0; i < whitePawns->get_size(); i++)
						if (whitePawns->get_row(i) == rows - 1)
							whitePawns->promote(i, whiteQueen, board, delta);


					for (int i = 0; i < blackPawns->get_size(); i++)
						if (blackPawns->get_row(i) == 0)
							blackPawns->promote(i, blackQueen, board, delta);



					if (rules->guns_enabled) {
						blackPawns->gun->change_boss(nullptr);
						whitePawns->gun->change_boss(nullptr);
					}


				}

			}

			if (event->button.button == SDL_BUTTON_RIGHT) {
				if (rules->guns_enabled) {
					if (blackPawns->get_selected() != UNSELECTED)
						blackPawns->gun->add_bullet(UP);

					if (whitePawns->get_selected() != UNSELECTED)
						whitePawns->gun->add_bullet(DOWN);
				}
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


	for (Pawns *p : white_pieces)
		p->render(&camera->camera_rect, renderer, 0);

	for (Pawns *p : black_pieces)
		p->render(&camera->camera_rect, renderer, 0);


	if (rules->guns_enabled) {
		whitePawns->gun->render(&camera->camera_rect, renderer);
		blackPawns->gun->render(&camera->camera_rect, renderer);
	}


	for (Buttons *b : buttons)
		b->render_if_visible(renderer);


	SDL_RenderPresent(renderer);
}


Game::~Game()
{
	for (Pawns* p : white_pieces)
		delete p;

	for (Pawns* p : black_pieces)
		delete p;

	white_pieces.clear();
	black_pieces.clear();

	delete tiles;
	delete board;
	delete window;
}
