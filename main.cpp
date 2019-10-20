#include "Game.h"




int main(int args, char* argv[])
{
	Game *game = new Game();

	game->init();
	game->load_assets();

    while(game->running)
    {
		game->clock();
		game->controlls();
		game->move();
		game->render();
    }



	return 0;
}