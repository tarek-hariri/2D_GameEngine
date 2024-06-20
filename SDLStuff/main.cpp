#include "Game.h"

int main(int argc, char* argv[]) {
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Game game = Game();
	if (!game.init("Baller", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 768, 640, false))
		return 0;

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	while (game.running()) {
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

		game.HandleEvents();
		game.Update(deltaTime);
		game.Render();

		if (frameDelay > deltaTime) {
			SDL_Delay(frameDelay - deltaTime);
		}
	}

	game.Clean();

	return 0;
}