#include "Game.hpp"

Game *game = nullptr;

int main(int argc, char **argv) {
	game = new Game();

	while (game->GetMainLoop() == 1) {
		game->Step();
		game->Draw();
	}

	game->Ending();
	delete game;
	return 0;
}