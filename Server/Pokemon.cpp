#include "Pokemon.h"

Pokemon::Pokemon(int speciesID){
	this->ivs = generateRandomIVs();
	this->nature = generateRandomNature();
	this->speciesID = speciesID;
}

Pokemon::IVs Pokemon::generateRandomIVs() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, 31);

	return IVs{ distr(gen), distr(gen), distr(gen), distr(gen), distr(gen), distr(gen) };
}

NATURE Pokemon::generateRandomNature(){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, 24);
	return static_cast<NATURE>(distr(gen));
}
