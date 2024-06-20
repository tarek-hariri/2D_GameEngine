#pragma once

#include "Game.h"
#include "Tile.h"
#include  "TextureManager.h"
#include "Settings.h"
#include <vector>


class TileMap {
public:
	TileMap(int width, int height);

	void Render();

	Tile* getTile(int x, int y);
	int getWidth();
	int getHeight();
	
private:
	SDL_Rect srcRect, destRect;

	std::vector<std::vector<Tile*>> tiles;
	int width, height;
	SDL_Texture* grass_1_texture; // GROSS, GROSS, GROSS!!!
};