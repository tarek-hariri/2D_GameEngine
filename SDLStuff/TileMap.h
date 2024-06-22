#pragma once

#include "Game.h"
#include "Tile.h"
#include  "TextureManager.h"
#include "PerlinNoiseGenerator.h"
#include "Settings.h"
#include <vector>
#include "Camera.h"


class TileMap {
public:
	TileMap(int width, int height);

	void Render(Camera* camera);

	Tile* getTile(int x, int y);
	int getWidth();
	int getHeight();
	
private:
	SDL_Rect srcRect, destRect;

	void generateMap(std::vector<std::vector<float>>& heightMap, const std::vector<std::vector<float>>& moistureMap);

	std::vector<std::vector<Tile*>> tiles;
	int width, height;
	SDL_Texture* grass_1_texture; // GROSS, GROSS, GROSS!!!
	SDL_Texture* water_texture;   // seriously, this is yucky. Figure out a better way to load and store these (at least don't hard code it mane)
};