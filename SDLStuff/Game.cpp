#include "Game.h"
#include "Player.h"
#include "TextureManager.h"


TileMap* tileMap;
SDL_Renderer* Game::renderer = nullptr;
std::unordered_map<Uint32, Player*> players;
Camera* Game::camera = new Camera;


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

				players.insert_or_assign(desc.nUniqueID, new Player(std::string("assets/redsinglesprite.png"), desc.xPos, desc.yPos, tileMap, 14, 19, &queuedMovements, desc.nUniqueID, std::string("assets/red_sprites/")));

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

			}
		}
	}

	if (waitingForConnection) {
		return;
	}

	// Update camera position
	camera->update(players[nPlayerID]->getWorldX(), players[nPlayerID]->getWorldY());

	// Update each player
	for (auto it : players)
		it.second->Update(deltaTime, camera);

}

void Game::Render() {
	SDL_RenderClear(renderer);

	// Draw the map
	tileMap->Render(camera);
	for (auto it : players)
		it.second->Render();
	SDL_RenderPresent(renderer);
}

void Game::Clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned. So sanitary." << std::endl;
}
