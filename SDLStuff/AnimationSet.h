#pragma once
#include "Game.h"
#include <unordered_map>

class AnimationSet {
public:
	AnimationSet();

	AnimationSet(
		const std::vector<SDL_Texture*>& walkNorth,
		const std::vector<SDL_Texture*>& walkEast,
		const std::vector<SDL_Texture*>& walkSouth,
		const std::vector<SDL_Texture*>& walkWest,
		SDL_Texture* standNorth,
		SDL_Texture* standEast,
		SDL_Texture* standSouth,
		SDL_Texture* standWest
	);

	SDL_Texture* getWalking(DIRECTION dir, bool nextFrame);
	SDL_Texture* getStanding(DIRECTION dir);



private:
	std::unordered_map<DIRECTION, SDL_Texture*> standing;
	std::unordered_map<DIRECTION, std::vector<SDL_Texture*>> walking;

	int frame;

	// If 1, frames will be played in ascending order. If -1, in descending order. 
	int direction;
};