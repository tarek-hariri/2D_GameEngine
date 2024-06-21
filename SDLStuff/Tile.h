#pragma once
#include "TERRAIN.h"

class Tile {
public:
	Tile(TERRAIN terrain) { this->terrain = terrain; }
	TERRAIN getTerrain() { return terrain; }
	void setTerrain(TERRAIN terrain) { this->terrain = terrain; }

private:
	TERRAIN terrain;
};