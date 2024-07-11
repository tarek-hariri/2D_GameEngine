#include "EncounterTile.h"


EncounterTile::EncounterTile(std::string textureSheet, int initialX, int initialY, int spriteWidth, int spriteHeight)
: GameObject::GameObject(textureSheet, initialX, initialY, spriteWidth, spriteHeight){

}

bool EncounterTile::checkEncounter(){
	return true;
}
