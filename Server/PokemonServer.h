#pragma once


#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <regex>


#include "NetCommon.h"



class PokemonServer : public olc::net::server_interface<GameMsg> {
public:
	PokemonServer(uint16_t nPort) : olc::net::server_interface<GameMsg>(nPort) {
	}

	std::unordered_map<uint32_t, sPlayerDescription> playerRoster;
	std::vector<uint32_t> garbageIDs;

protected:
	bool OnClientConnect(std::shared_ptr < olc::net::connection<GameMsg>> client) override {
		return true;
	}

	void OnClientValidated(std::shared_ptr<olc::net::connection<GameMsg>> client) override {
		olc::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Client_Accepted;
		client->Send(msg);
	}

	void OnClientDisconnect(std::shared_ptr<olc::net::connection<GameMsg>> client) override;

	void OnMessage(std::shared_ptr<olc::net::connection<GameMsg>> client, olc::net::message<GameMsg>& msg) override;

};