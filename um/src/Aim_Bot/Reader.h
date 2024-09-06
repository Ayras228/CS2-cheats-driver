#pragma once

#include "../Vectors/Vectors.h"

#include "../driver_manager.h"

#include"../CS2_Data/client_dll.hpp"
#include"../CS2_Data/offsets.hpp"
#include"../CS2_Data/buttons.hpp"

#include <vector>



class C_CSPlayerPawn
{
public:
	int health, team;

	Vector3 Position;

	uintptr_t pCSPlayerPawn;
};

inline C_CSPlayerPawn CCSPlayerPawn;


class Reader
{
public:
	Reader(const std::uintptr_t& client);

	uintptr_t client;

	std::vector<C_CSPlayerPawn> playerList;

	void ReadPlayers(DriverManager& driver_manager, const std::uintptr_t& client);
};