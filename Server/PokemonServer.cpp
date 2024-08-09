#include "PokemonServer.h"

using namespace subprocess;



void PokemonServer::OnMessage(std::shared_ptr<olc::net::connection<GameMsg>> client, olc::net::message<GameMsg>& msg){
	if (!garbageIDs.empty()) {
		for (auto pid : garbageIDs) {
			olc::net::message<GameMsg> msg;
			msg.header.id = GameMsg::Game_RemovePlayer;
			msg << pid;
			std::cout << "Removing " << pid << "\n";
			MessageAllClients(msg);
		}
		garbageIDs.clear();
	}
	
	
	switch (msg.header.id) {
	case GameMsg::Client_RegisterWithServer: {

		// Set player's initial position to 0,0
		sPlayerDescription desc = sPlayerDescription{ client->GetID(), 0, 0 };
		playerRoster.insert_or_assign(desc.nUniqueID, desc);

		olc::net::message<GameMsg> msgSendID;
		msgSendID.header.id = GameMsg::Client_AssignID;
		msgSendID << desc.nUniqueID;
		MessageClient(client, msgSendID);

		olc::net::message<GameMsg> msgAddPlayer;
		msgAddPlayer.header.id = GameMsg::Game_AddPlayer;
		msgAddPlayer << desc;
		MessageAllClients(msgAddPlayer);

		for (const auto& player : playerRoster) {
			olc::net::message<GameMsg> msgAddOtherPlayers;
			msgAddOtherPlayers.header.id = GameMsg::Game_AddPlayer;
			msgAddOtherPlayers << player.second;
			MessageClient(client, msgAddOtherPlayers);
		}
		break;
	}

	case GameMsg::Client_UnregisterWithServer: {
		break;
	}
	case GameMsg::Game_UpdatePlayer:{
		MessageAllClients(msg, client);
		break;
	}
	case GameMsg::Client_BeginEncounter:{
		// determine encounter (lookup player position in worldmap)
		
		// load player's team

		// initialize simulation
		std::filesystem::current_path("pokemon-showdown");
		auto sim = Popen("node pokemon-showdown simulate-battle", input{ PIPE }, output{ PIPE });

		auto msg = ">start {\"formatid\":\"gen7randombattle\"}\n";
		sim.send(msg, strlen(msg));

		auto msg2 = ">player p1 {\"name\":\"Alice\"}\n";
		sim.send(msg2, strlen(msg2));

		auto msg3 = ">player p2 {\"name\":\"Bob\"}\n";
		sim.send(msg3, strlen(msg3));

		olc::net::message<GameMsg> msgInitializedBattle;
		msgInitializedBattle.header.id = GameMsg::Server_InitializedBattle;
		
		MessageClient(client, msgInitializedBattle);


		int turn = 1;
		std::string thisLine;
		int thisCharacter;

		// Battle loop
		while (true) {
			while ((thisCharacter = fgetc(sim.output())) != EOF) {
				//print the character to a string
				printf("%c", thisCharacter);
				if (thisCharacter == '\n')
					thisLine = "";
				else
					thisLine += thisCharacter;

				if (turn <= 9) {
					if (std::regex_match(thisLine, std::regex("\\|turn\\|[0-9]{1}")))
						break;
				}
				else if (turn <= 99) {

				}
				else if (turn <= 999) {

				}
				// Quickest stall matchup
				else {

				}
			}

			// send battle info to client

			// Gather input from both sides, format it into message, send
			std::string input;

			sim.send(input.c_str(), input.length());
		}

	}

	}



}


void PokemonServer::OnClientDisconnect(std::shared_ptr<olc::net::connection<GameMsg>> client) {
	if (client) {
		if (playerRoster.find(client->GetID()) == playerRoster.end()) {
			// client never added to roster, so just let it disappear
		}
		else {
			auto& pd = playerRoster[client->GetID()];
			std::cout << "[UNGRACEFUL REMOVAL]:" + std::to_string(pd.nUniqueID) + "\n";
			playerRoster.erase(client->GetID());
			garbageIDs.push_back(client->GetID());
		}
	}
}


int main() {
	PokemonServer server(60000);
	server.Start();


	while (1) {
		server.Update(-1, true);
	}

	return 0;
}
