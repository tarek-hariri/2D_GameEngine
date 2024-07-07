#include "AnimationSet.h"


AnimationSet::AnimationSet(const std::vector<SDL_Texture*>& walkNorth, const std::vector<SDL_Texture*>& walkEast, const std::vector<SDL_Texture*>& walkSouth, const std::vector<SDL_Texture*>& walkWest, SDL_Texture* standNorth, SDL_Texture* standEast, SDL_Texture* standSouth, SDL_Texture* standWest)
{
	this->standing = std::unordered_map<DIRECTION, SDL_Texture*>();
	this->walking = std::unordered_map <DIRECTION, std::vector<SDL_Texture*>>();

	this->standing.insert({ DIRECTION::NORTH, standNorth });
	this->standing.insert({ DIRECTION::EAST, standEast });
	this->standing.insert({ DIRECTION::SOUTH, standSouth});
	this->standing.insert({ DIRECTION::WEST, standWest});

	this->walking.insert({ DIRECTION::NORTH, walkNorth });
	this->walking.insert({ DIRECTION::EAST, walkEast });
	this->walking.insert({ DIRECTION::SOUTH, walkSouth });
	this->walking.insert({ DIRECTION::WEST, walkWest });

	frame = 0;
	direction = 1;
}

AnimationSet::AnimationSet() {}


SDL_Texture* AnimationSet::getWalking(DIRECTION dir, bool nextFrame)
{
	if (nextFrame)
		frame = frame + direction;

	if (frame == 0 && direction == -1)
		direction = 1;
	else if (frame == (this->walking.at(dir).size() - 1))
		direction = -1;
	return this->walking.at(dir).at(frame);
}

SDL_Texture* AnimationSet::getStanding(DIRECTION dir)
{
	frame = 0;
	direction = 1;
	return this->standing.at(dir);
}
