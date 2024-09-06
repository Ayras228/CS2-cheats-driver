#pragma once
#include <thread> // Для роботи з потоками
#include <windows.h> // Для GetAsyncKeyState

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream> // Для типів даних, таких як uint32_t
#include <vector>
#include<list>

#include"../CS2_Data/client_dll.hpp"
#include"../CS2_Data/offsets.hpp"
#include"../CS2_Data/buttons.hpp"

#include "../driver_manager.h"
#include"../Vectors/Vectors.h"
#include"../Aim_Bot/Reader.h"

#include"targeting_logic.cpp"



namespace _offset 
{
	//offset.dll
	const auto dwLocalPlayerPawn = cs2_dumper::offsets::client_dll::dwLocalPlayerPawn;
	const auto dwEntityList = cs2_dumper::offsets::client_dll::dwEntityList;
	const auto dwViewMatrix = cs2_dumper::offsets::client_dll::dwViewMatrix;
	const auto dwLocalPlayerController = cs2_dumper::offsets::client_dll::dwLocalPlayerController;

	//client.dll
	const auto m_hPlayerPawn = cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn;
	const auto m_iHealth = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth;
	const auto m_vOldOrigin = cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin;
	const auto m_iTeamNum = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum;
	const auto m_entitySpottedState = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState;
	const auto m_lifeState = cs2_dumper::schemas::client_dll::C_BaseEntity::m_lifeState;
	const auto m_modelState = cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState;
	const auto m_pGameSceneNode = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode;
	
}

static void aim_bot(DriverManager& driver_manager, const std::uintptr_t& client)
{
	Reader reader(client);
	while (true)
	{
		// sleep for 1ms to save cpu %
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		
		reader.ReadPlayers(driver_manager,client);
		// get our view_matrix
		auto view_matrix = driver_manager.read_memory<view_matrix_t>(reader.client + _offset::dwViewMatrix);
		// create our playerPositions vector, although i would recommend moving this out of the loop.
		std::vector<Vector3> playerPositions;

		// clear our playerPositions vector to remove old players
		playerPositions.clear();
		
		for (const auto& player : reader.playerList)
		{
			// get the 3D position of the player we're CURRENTLY looping through.
			Vector3 playerPosition = driver_manager.read_memory<Vector3>(player.pCSPlayerPawn + _offset::m_vOldOrigin);

			// create a headPosition Vector3, this is kind of ghetto but it works fine.
			auto x = playerPosition.get_x();
			auto y = playerPosition.get_y();
			auto z = playerPosition.get_z();
			Vector3 headPos = { x += 0.0, y += 0.0, z += 65.0f };

			// create our out variables for the world_to_screen function.
			Vector3 f, h;

			if (Vector3::world_to_screen(view_matrix, playerPosition, f) &&
				Vector3::world_to_screen(view_matrix, headPos, h))
			{
				// add the filtered player to our vector
				playerPositions.push_back(h);
			}
		}

		// check if the user is holding the right mouse button.
		if (GetAsyncKeyState(VK_LSHIFT))
		{
			Vector2 center_of_screen{ (float)GetSystemMetrics(0) / 2, (float)GetSystemMetrics(1) / 2 };
			// find the closest player and store it in a variable
			auto closest_player = findClosest(playerPositions, center_of_screen);

			// move the mouse to the player
			MoveMouseToPlayer(closest_player, center_of_screen); //closest_player
		}
	}
}
