#pragma once

#include "GameObject.h"

class EncounterTile : public GameObject {
public:
	EncounterTile(std::string textureSheet, int initialX, int initialY, int spriteWidth, int spriteHeight);
	
	// TO DO: Implement constructor that takes params for encounter info, type of tile, animation set, etc...

	bool checkEncounter();
};