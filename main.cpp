#include <SDL3/SDL.h>
#include "Game.h"
#include "Logger.h"

int main() {
	Game game; // Game init
	Logger log(game);
	if (!game.loadLog(log)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Logger loading error");
		return -1;
	}
	while (game.isRunning()) {
		game.input();

		game.update();
		
		game.draw();
	}
	return 0;
}