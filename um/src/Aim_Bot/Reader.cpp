#include "Reader.h"


Reader::Reader(const std::uintptr_t& client)
{
	this->client = client;
}

void Reader::ReadPlayers(DriverManager& driver_manager, const std::uintptr_t& client)
{
	// clear playerList
	playerList.clear();

	auto entityList = driver_manager.read_memory<std::uintptr_t>(client + cs2_dumper::offsets::client_dll::dwEntityList);

	if (!entityList)
		return;

	auto localPawn = driver_manager.read_memory<std::uintptr_t>(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);


	// check swedz video for an explanation of this, i do not have the patience to write out all those comments :(
	for (int i = 0; i <= 64; ++i)
	{
		std::uintptr_t list_entry1 = driver_manager.read_memory<uintptr_t>(entityList + (8 * (i & 0x7FFF) >> 9) + 16);

		std::uintptr_t playerController = driver_manager.read_memory<std::uintptr_t>(list_entry1 + 120 * (i & 0x1FF));

		uint32_t playerPawn = driver_manager.read_memory<uint32_t>(playerController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);

		std::uintptr_t list_entry2 = driver_manager.read_memory<std::uintptr_t>(entityList + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);

		std::uintptr_t pCSPlayerPawnPtr = driver_manager.read_memory<std::uintptr_t>(list_entry2 + 120 * (playerPawn & 0x1FF));

		int health = driver_manager.read_memory<int>(pCSPlayerPawnPtr + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);

		if (health <= 0 || health > 100)
			continue;

		int team = driver_manager.read_memory<int>(pCSPlayerPawnPtr + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

		//if (team == driver_manager.read_memory<int>(localPawn + _offset::m_iTeamNum))
			//continue;

		// save the address of the pawn we're on for later use, possibly reading positions.
		CCSPlayerPawn.pCSPlayerPawn = pCSPlayerPawnPtr;

		// push back the entity we're on now, as in save them for later so we can loop through them.
		playerList.push_back(CCSPlayerPawn);
	}
}