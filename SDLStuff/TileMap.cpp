#include "TileMap.h"


TileMap::TileMap(int width, int height) {
	this->width = width;
	this->height = height;
	tiles = std::vector<std::vector<Tile*>>(
		width, std::vector<Tile*>(height, new Tile(TERRAIN::GRASS_1)));
	
	grass_1_texture = TextureManager::LoadTexture("assets/palletgrass.png");
	
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = TILE_SIZE;
	srcRect.h = TILE_SIZE;

	destRect.w = SCALED_TILE_SIZE;
	destRect.h = SCALED_TILE_SIZE;
}

void TileMap::Render() {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			destRect.x = x * SCALED_TILE_SIZE;
			destRect.y = y * SCALED_TILE_SIZE;
			TextureManager::Draw(grass_1_texture, srcRect, destRect);
		}
	}
}


Tile* TileMap::getTile(int x, int y) {
	return tiles.at(x).at(y);
}

int TileMap::getWidth() {
	return width;
}

int TileMap::getHeight() {
	return height;
}