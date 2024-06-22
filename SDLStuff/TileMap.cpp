#include "TileMap.h"


TileMap::TileMap(int width, int height) {
	this->width = width;
	this->height = height;
	tiles = std::vector<std::vector<Tile*>>(
		width, std::vector<Tile*>(height, nullptr));
	
	PerlinNoiseGenerator heightGen;
	PerlinNoiseGenerator moistureGen;
	moistureGen.octaves = 10;
	moistureGen.lacunarity = 1.2f;
	moistureGen.persistance = 0.3f;
	moistureGen.scale = 75;

	std::vector<std::vector<float>> heightMap = heightGen.generateNoiseMap(true, MAP_SIZE, MAP_SIZE);
	std::vector<std::vector<float>> moistureMap = moistureGen.generateNoiseMap(false, MAP_SIZE, MAP_SIZE);

	generateMap(heightMap, moistureMap);

	grass_1_texture = TextureManager::LoadTexture("assets/palletgrass.png");
	water_texture = TextureManager::LoadTexture("assets/water.png");
	
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = TILE_SIZE;
	srcRect.h = TILE_SIZE;

	destRect.w = SCALED_TILE_SIZE;
	destRect.h = SCALED_TILE_SIZE;
}

void TileMap::Render(Camera* camera) {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			destRect.x = camera->getWorldStartX() + x * SCALED_TILE_SIZE;
			destRect.y = camera->getWorldStartY() + y * SCALED_TILE_SIZE;
			switch (tiles[x][y]->getTerrain()) {
				case GRASS_1:
					TextureManager::Draw(grass_1_texture, srcRect, destRect);
					break;
				case WATER_1:
					TextureManager::Draw(water_texture, srcRect, destRect);
					break;
			}
		}
	}
}

void TileMap::generateMap(std::vector<std::vector<float>>& heightMap, const std::vector<std::vector<float>>& moistureMap) {
	// Could check if heightMap and moistureMap are same dimensions... if you're a PUSSY! (I am lazy and will pay for this later)

	float islandMax = FLT_MIN;
	float islandMin = FLT_MAX;

	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			float sample = heightMap[x][y];

			islandMin = sample < islandMin ? sample : islandMin;
			islandMax = sample > islandMax ? sample : islandMax;
		}
	}

	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			heightMap[x][y] = PerlinNoiseGenerator::inverseLerp(islandMin, islandMax, heightMap[x][y]);

			if (heightMap[x][y] >= .5) {
				tiles[x][y] = new Tile(TERRAIN::GRASS_1);
			}
			else {
				tiles[x][y] = new Tile(TERRAIN::WATER_1);
			}
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

