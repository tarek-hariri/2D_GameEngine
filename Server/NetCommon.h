#pragma once
#include "olcPGEX_Network.h"

enum class GameMsg : uint32_t {
	Server_GetStatus,
	Server_GetPing,

	Client_Accepted,
	Client_AssignID,
	Client_RegisterWithServer,
	Client_UnregisterWithServer,

	Game_AddPlayer,
	Game_RemovePlayer,
	Game_UpdatePlayer,

	Client_BeginEncounter,
	Client_BeginBattle,
	Client_MakeMove,

	Server_InitializedBattle
};

struct sPlayerDescription {
	uint32_t nUniqueID;
	int xPos;
	int yPos;
};