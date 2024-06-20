#pragma once

#include "SDL.h"
#undef main
#include "SDL_image.h"
#include <iostream>
#include <unordered_map>
#include "TileMap.h"
#include "Settings.h"
#include "../Client/PokemonClient.h"

class Game {
public:
	Game();
	~Game();

	// Initializes game screen stuff
	bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void HandleEvents();
	void Update(float deltaTime);
	void Render();
	void Clean();
	bool running() { return isRunning; }

	static SDL_Renderer* renderer;

private:
	// Connects to server and initiailizes player
	bool Connect();
	

	std::unordered_map<uint32_t, sPlayerDescription> queuedMovements;

	bool isRunning;
	bool waitingForConnection = true;
	
	PokemonClient client;
	SDL_Window* window;

	int frameCount, timerFPS, lastFrame;
	uint32_t nPlayerID = 0;

};