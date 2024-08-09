#include "Game.h"
#include "EncounterTile.h"
#include "Player.h"
#include "TextureManager.h"


TileMap* tileMap;
SDL_Renderer* Game::renderer = nullptr;
std::unordered_map<Uint32, Player*> players;
Camera* Game::camera = new Camera;

std::vector<EncounterTile*> game_objects;


Game::Game() {
}
Game::~Game() {

}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen){
	int flags = 0;
	if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;

	if (!(SDL_Init(SDL_INIT_EVERYTHING) == 0))
		return false;

	std::cout << "Subsystems Initialized" << std::endl;
	window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

	if (!window)
		return false;

	std::cout << "Window created!" << std::endl;
	renderer = SDL_CreateRenderer(window, -1, 0);
	
	if (!renderer)
		return false;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	std::cout << "Renderer created!" << std::endl;

	
	if (!Connect()) {
		std::cout << "Could not connect." << std::endl;
		return false;
	}
	std::cout << "Connected to server!" << std::endl;

	queuedMovements = std::unordered_map<uint32_t, sPlayerDescription>();
	tileMap = new TileMap(MAP_SIZE, MAP_SIZE);
	isRunning = true; // tentatively

	game_objects.push_back(new EncounterTile("assets/tallgrass.png", 2, 3, 16, 16));

	return true;
}

bool Game::Connect(){
	if (client.Connect("127.0.0.1", 60000)) {
		return true;
	}

	return false;
}

void Game::HandleEvents() {

	// Maybe you'll want to allow some inputs while connecting in the future -- do that here
	if (waitingForConnection) {
		return;
	}
	SDL_Event event;
	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		}
	}

	bool moving = false;

	// Might want to change logic to prioritize direction that aligns with last move?
	if (keystates[SDL_SCANCODE_UP]) {
		moving = players[nPlayerID]->Move(0, -1);
		if (!moving) {
			sPlayerDescription desc = sPlayerDescription{ nPlayerID, players[nPlayerID]->getX(), players[nPlayerID]->getY() - 1 };
			queuedMovements.insert_or_assign(nPlayerID, desc);

		}
	}
	else if (keystates[SDL_SCANCODE_DOWN]) {
		moving = players[nPlayerID]->Move(0, 1);

		if (!moving) {
			sPlayerDescription desc = sPlayerDescription{ nPlayerID, players[nPlayerID]->getX(), players[nPlayerID]->getY() + 1 };
			queuedMovements.insert_or_assign(nPlayerID, desc);
		}
	}
	else if (keystates[SDL_SCANCODE_RIGHT]) {
		moving = players[nPlayerID]->Move(1, 0);

		if (!moving) {
			sPlayerDescription desc = sPlayerDescription{ nPlayerID, players[nPlayerID]->getX() + 1, players[nPlayerID]->getY()};
			queuedMovements.insert_or_assign(nPlayerID, desc);
		}
	}
	else if (keystates[SDL_SCANCODE_LEFT]) {
		if (!moving) {
			sPlayerDescription desc = sPlayerDescription{ nPlayerID, players[nPlayerID]->getX() - 1, players[nPlayerID]->getY()};
			queuedMovements.insert_or_assign(nPlayerID, desc);
		}
		moving = players[nPlayerID]->Move(-1, 0);
	}
	// No movements, empty da queue
	else {
		queuedMovements.erase(nPlayerID);
	}
	


	if (moving) {
		olc::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Game_UpdatePlayer;

		sPlayerDescription desc = sPlayerDescription{ nPlayerID, players[nPlayerID]->getX(), players[nPlayerID]->getY() };
		msg << desc;

		client.Send(msg);
	}

	// If a queued move was executed for this player-controlled character
	else if (players[nPlayerID]->getExecutedQueuedMove()){
		olc::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Game_UpdatePlayer;

		sPlayerDescription desc = sPlayerDescription{ nPlayerID, players[nPlayerID]->getX(), players[nPlayerID]->getY() };
		msg << desc;

		client.Send(msg);
		players[nPlayerID]->resetExecutedQueuedMove();
	}
	
}

void Game::Update(float deltaTime) {
	if (client.IsConnected()) {
		while (!client.Incoming().empty()) {
			auto msg = client.Incoming().pop_front().msg;

			switch (msg.header.id) {
			case(GameMsg::Client_Accepted): {
				std::cout << "Server accepted client - you're in!\n";
				olc::net::message<GameMsg> msg;
				msg.header.id = GameMsg::Client_RegisterWithServer;
				
				// Send whatever info about the player you want client to know...
				// I think position should be sent by server to client

				client.Send(msg);
				break;
			}
			case(GameMsg::Client_AssignID): {
				msg >> nPlayerID;
				std::cout << "Assigned Client ID = " << nPlayerID << "\n";
				break;
			}
			case(GameMsg::Game_AddPlayer): {
				sPlayerDescription desc;
				msg >> desc;

				players.insert_or_assign(desc.nUniqueID, new Player(std::string("assets/redsinglesprite.png"), desc.xPos, desc.yPos, tileMap, 16, 24, &queuedMovements, desc.nUniqueID, std::string("assets/red_sprites/")));

				if (waitingForConnection && desc.nUniqueID == nPlayerID) {
					// Our player is in! 
					// If you were waiting to render the world or something until you were in, do that here.
					waitingForConnection = false;
				}

				break;
			}
			case(GameMsg::Game_RemovePlayer): {
				uint32_t nRemovalID = 0;
				msg >> nRemovalID;
				players.erase(nRemovalID);
				break;
			}
			case(GameMsg::Game_UpdatePlayer): {
				sPlayerDescription desc;
				msg >> desc;

				// Move player (process server input)
				int currentX = players[desc.nUniqueID]->getX();
				int currentY = players[desc.nUniqueID]->getY();
				bool test = players[desc.nUniqueID]->Move(desc.xPos - currentX, desc.yPos - currentY);
				
				// Player still finishing previous movement, can't process update yet so let's store it and execute it when we can
				// queuedMovement holds the latest movement update for each player, so if one is already stored it will overwrite it (although you really shouldn't get that congested)
				if (!test) {
					queuedMovements.insert_or_assign(desc.nUniqueID, desc);
				}
			}
			case(GameMsg::Server_InitializedBattle): {

			}

			}
		}
	}

	if (waitingForConnection) {
		return;
	}

	// TO DO: THIS IS NOT GOOD WHEN YOU HAVE A BIG MAP!!!!!! Figure out a way to check the "tile" the player is standing on, once you implement a World/consolidate the map and stuff into one entity.
	for (auto tile : game_objects) {
		if (players[nPlayerID]->getX() == tile->getX() && players[nPlayerID]->getY() == tile->getY()) {
			if (tile->checkEncounter()) {
				// Send message to server requesting an encounter for the given tile
				olc::net::message<GameMsg> msg;
				msg.header.id = GameMsg::Client_BeginEncounter;
				client.Send(msg);
			}
		}
	}

	// Update camera position
	camera->update(players[nPlayerID]->getWorldX(), players[nPlayerID]->getWorldY());
	
	// Update each world object
	for (auto it : game_objects)
		it->Update(deltaTime, camera);

	// Update each player
	for (auto it : players)
		it.second->Update(deltaTime, camera);

}

void Game::Render() {
	SDL_RenderClear(renderer);

	// Draw the map
	tileMap->Render(camera);

	for (auto it : game_objects)
		it->Render();

	for (auto it : players)
		it.second->Render();
	
	// okay the issue is that camera... figure out how to pupt game objects into the tilemap/world
	SDL_RenderPresent(renderer);
}

void Game::Clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned. So sanitary." << std::endl;
}
