#pragma once
#include "NATURE.h"
#include <random>

class Pokemon {
public:
	Pokemon(int speciesID);

private:
	struct IVs {
		int hp;
		int attack;
		int defense;
		int speed;
		int spAtk;
		int spDef;
	};
	static IVs generateRandomIVs();
	static NATURE generateRandomNature();

	int speciesID;
	IVs ivs;
	NATURE nature;
};