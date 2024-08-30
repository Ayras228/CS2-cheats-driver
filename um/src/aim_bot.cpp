#pragma once
#include <thread> // Для роботи з потоками
#include <windows.h> // Для GetAsyncKeyState

#include <limits> // Додаємо цей заголовок для використання std::numeric_limits
#include <cmath>   // Для std::sqrt і std::pow  
#include <cfloat>  // Для FLT_MAX

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream> // Для типів даних, таких як uint32_t
#include <vector>
#include<list>

#include "driver_manager.h"
#include"cs2_data/client_dll.hpp"
#include"cs2_data/offsets.hpp"
#include"cs2_data/buttons.hpp"



#include "math/vector.hpp"

// Структура для зберігання координат у тривимірному просторі
class Vector2
{
public:
	float x, y;
public:
	Vector2() :x(0.f), y(0.f) {}
	Vector2(float x_, float y_) :x(x_), y(y_) {}
	
	Vector2 operator+(Vector2 Vector2_)
	{
		return { x + Vector2_.x,y + Vector2_.y };
	}
	Vector2 operator-(Vector2 Vector2_)
	{
		return { x - Vector2_.x,y - Vector2_.y };
	}
	Vector2 operator*(Vector2 Vector2_)
	{
		return { x * Vector2_.x,y * Vector2_.y };
	}
	Vector2 operator/(Vector2 Vector2_)
	{
		return { x / Vector2_.x,y / Vector2_.y };
	}
	Vector2 operator*(float n)
	{
		return { x / n,y / n };
	}
	Vector2 operator/(float n)
	{
		return { x / n,y / n };
	}
	bool operator==(Vector2 Vector2_)
	{
		return x == Vector2_.x && y == Vector2_.y;
	}
	bool operator!=(Vector2 Vector2_)
	{
		return x != Vector2_.x || y != Vector2_.y;
	}
	float Length()
	{
		return sqrtf(powf(x, 2) + powf(y, 2));
	}
	float DistanceTo(const Vector2& Pos)
	{
		return sqrtf(powf(Pos.x - x, 2) + powf(Pos.y - y, 2));
	}
};

class Vector3
{
public:
	float x, y, z;
public:
	Vector3() :x(0.f), y(0.f), z(0.f) {}
	Vector3(float x_, float y_, float z_) :x(x_), y(y_), z(z_) {}
	Vector3 operator+(Vector3 Vector3_)
	{
		return { x + Vector3_.x,y + Vector3_.y,z + Vector3_.z };
	}
	Vector3 operator-(Vector3 Vector3_)
	{
		return { x - Vector3_.x,y - Vector3_.y,z - Vector3_.z };
	}
	Vector3 operator*(Vector3 Vector3_)
	{
		return { x * Vector3_.x,y * Vector3_.y,z * Vector3_.z };
	}
	Vector3 operator/(Vector3 Vector3_)
	{
		return { x / Vector3_.x,y / Vector3_.y,z / Vector3_.z };
	}
	Vector3 operator*(float n)
	{
		return { x * n,y * n,z * n };
	}
	Vector3 operator/(float n)
	{
		return { x / n,y / n,z / n };
	}
	bool operator==(Vector3 Vector3_)
	{
		return x == Vector3_.x && y == Vector3_.y && z == Vector3_.z;
	}
	bool operator!=(Vector3 Vector3_)
	{
		return x != Vector3_.x || y != Vector3_.y || z != Vector3_.z;
	}
	float Length()
	{
		return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
	}
	float DistanceTo(const Vector3& Pos)
	{
		return sqrtf(powf(Pos.x - x, 2) + powf(Pos.y - y, 2) + powf(Pos.z - z, 2));
	}
	Vector3 RelativeAngle()
	{
		return {
			static_cast<float>(std::atan2(-z, std::hypot(x, y)) * (180.0f / M_PI)),
			static_cast<float>(std::atan2(y, x) * (180.0f / M_PI)),
			0.0f
		};
	}
};

// Функція для розрахунку кута огляду`
//inline Vector2 calculate_view_angles(const Vector3& local_position, const Vector3& enemy_position) {
//    Vector3 delta = {
//        enemy_position.x - local_position.x,
//        enemy_position.y - local_position.y,
//        enemy_position.z - local_position.z
//    };
//
//    float hypotenuse = std::sqrt(delta.x * delta.x + delta.y * delta.y);
//
//    Vector2 angles;
//    angles.x = std::atan2(delta.y, delta.x) * (180.0f / static_cast<float>(M_PI)); // Yaw
//    angles.y = std::atan2(delta.z, hypotenuse) * (180.0f / static_cast<float>(M_PI)); // Pitch
//    //angles.x = 24.0f;
//    //angles.y = -30.0f;
//    return angles;
//}
struct BoneJointData
{
	Vector3 Pos;
	char pad[0x14];
};

struct BoneMatrix
{
	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;
};

enum BoneIds : DWORD
{
	Waist = 0,
	Neck = 5,
	Head = 6,
	ShoulderLeft = 8,
	ForeLeft = 9,
	HandLeft = 11,
	ShoulderRight = 13,
	ForeRight = 14,
	HandRight = 16,
	KneeLeft = 23,
	FeetLeft = 24,
	KneeRight = 26,
	FeetRight = 27
};

inline std::vector<DWORD> BoneIdsVec = { 
	Waist ,
	Neck ,
	Head ,
	ShoulderLeft ,
	ForeLeft ,
	HandLeft ,
	ShoulderRight ,
	ForeRight ,
	HandRight ,
	KneeLeft ,
	FeetLeft ,
	KneeRight ,
	FeetRight  
};


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
//
//inline std::list<Vector3> read_bones (DriverManager& driver_manager, DWORD64 bone_addres)
//{
//	const int maxBones = 27;
//	const int boneStep = 32;
//	const int readSize = bone_addres + maxBones * boneStep + 16; // same size as in the C# code
//
//
//	const DWORD64 boneBytes = driver_manager.read_memory<DWORD64>(
//		bone_addres + maxBones * boneStep + 16);
//
//
//	std::list<Vector3> bones;
//	
//
//
//	for (DWORD boneId:BoneIdsVec)
//	
//	{ 
//		float x = *reinterpret_cast<float*>(boneBytes + boneId * boneStep + 0);
//		float y = *reinterpret_cast<float*>(boneBytes + boneId * boneStep + 4);
//		float z = *reinterpret_cast<float*>(boneBytes + boneId * boneStep + 8);
//
//		bones.push_back(Vector3{ x, y, z });
//	}
//	return bones;
//}





inline Vector2 CalculateViewAngle(const Vector3& playerPos, const Vector3& enemyPos) {
	// Обчислення різниці координат
	Vector3 delta = Vector3(enemyPos.x - playerPos.x, enemyPos.y - playerPos.y, enemyPos.z - playerPos.z);
	//OppPos = enemyPos - playerPos;
	// Обчислення горизонтальної відстані
	float hypotenuse = sqrt(delta.x * delta.x + delta.y * delta.y);

	// Обчислення кутів
	float yaw = atan2(delta.y, delta.x) * 180.0f / M_PI;  // кут по горизонталі
	float pitch = atan2(delta.z, hypotenuse) * 180.0f / M_PI;  // кут по вертикалі

	// Нормалізація yaw в діапазон від -90 до 90
	if (yaw > 90) yaw -= 180;
	if (yaw < -90) yaw += 180;

	return Vector2(yaw, pitch);
}
inline Vector2 CalculateViewAngleAIMBOT(const Vector3& Local, const Vector3& LocalPos)
{
	float Yaw, Pitch;
	float Distance, Norm;
	Vector3 OppPos;

	int HotKey = VK_LBUTTON;
	float AimFov = 5;
	float Smooth = 0.7;
	Vector2 RCSScale = { 1.f,1.f };
	int RCSBullet = 1;

	OppPos =  LocalPos;

	Distance = sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2));

	Yaw = atan2f(OppPos.y, OppPos.x) * 57.295779513 - Local.y;
	Pitch = -atan(OppPos.z / Distance) * 57.295779513 - Local.x;
	/*Norm = sqrt(pow(Yaw, 2) + pow(Pitch, 2));
	if (Norm > AimFov)
		return Vector2(Yaw, Pitch);

	Yaw = Yaw * (1 - Smooth) + Local.y;
	Pitch = Pitch * (1 - Smooth) + Local.x;*/

	return Vector2(Yaw, Pitch);
	//// Recoil control
	//if (Local.Pawn.ShotsFired > RCSBullet)
	//{
	//	Vec2 PunchAngle;
	//	if (Local.Pawn.AimPunchCache.Count <= 0 && Local.Pawn.AimPunchCache.Count > 0xFFFF)
	//		return;
	//	if (!ProcessMgr.ReadMemory<Vec2>(Local.Pawn.AimPunchCache.Data + (Local.Pawn.AimPunchCache.Count - 1) * sizeof(Vector3), PunchAngle))
	//		return;

	//	Yaw = Yaw - PunchAngle.y * RCSScale.x;
	//	Pitch = Pitch - PunchAngle.x * RCSScale.y;
	//}

	//gGame.SetViewAngle(Yaw, Pitch);
}




static void aim_bot(DriverManager& driver_manager, const std::uintptr_t& client)
{
	
    while (true)
    {
        if (GetAsyncKeyState(VK_END))
        {
            break;
        }

        const auto local_player_pawn = driver_manager.read_memory<std::uintptr_t>(
            client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);

        if (local_player_pawn == 0)
        {
            continue;
        }


		Vector3 local_position = driver_manager.read_memory<Vector3>(
			local_player_pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_vecLastClipCameraPos);
		
		Vector2 EyeAngles = driver_manager.read_memory<Vector2>(
			local_player_pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_angEyeAngles);


		for (size_t i = 0; i < 64; i++)
		{
			
			DWORD64 player_pawn = driver_manager.read_memory<DWORD64>(
				local_player_pawn + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);

			DWORD64 dwLocalPlayerController = driver_manager.read_memory<DWORD64>(
				client + cs2_dumper::offsets::client_dll::dwLocalPlayerController);



			DWORD64 Pawn = driver_manager.read_memory<DWORD64>(
				player_pawn + dwLocalPlayerController);

			DWORD64 dwEntityList = driver_manager.read_memory<DWORD64>(
				client + cs2_dumper::offsets::client_dll::dwEntityList);

			DWORD64 EntityPawnListEntry = driver_manager.read_memory<DWORD64>(
				dwEntityList + 0x10 + 8 * ((Pawn & 0x7FFF) >> 9));


			//get pawn
			DWORD64 current_pawn = driver_manager.read_memory<DWORD64>(
				EntityPawnListEntry + 0x78 * (Pawn & 0x1FF));



			if (Pawn == 0)
			{
				continue;
			}
			
			DWORD64 current_control = driver_manager.read_memory<DWORD64>(Pawn + i * 0x78);

			if (current_control == 0)
			{
				continue;
			}
			
			DWORD64 GameSceneNode = driver_manager.read_memory<DWORD64>(
				local_player_pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

			DWORD64 boneMatrix = driver_manager.read_memory<DWORD64>(
				GameSceneNode + cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState 
				+ 0x80);

			BoneJointData BoneArray = driver_manager.read_memory<BoneJointData>(
				boneMatrix + 30 * sizeof(BoneJointData));

			Vector2 view_angles = CalculateViewAngleAIMBOT(local_position, BoneArray.Pos);
			driver_manager.write_memory(client + cs2_dumper::offsets::client_dll::dwViewAngles, view_angles);
		}
    }
}



class C_CSPlayerPawn
{
public:
	int health, team;

	my_Vector Position;

	uintptr_t pCSPlayerPawn;
};

inline C_CSPlayerPawn CCSPlayerPawn;


class Reader
{
public:
	Reader(const std::uintptr_t& client)
	{
		this->client = client;
	}

	uintptr_t client;

	std::vector<C_CSPlayerPawn> playerList;

	void FilterPlayers(DriverManager& driver_manager, const std::uintptr_t& client);
};
inline void Reader::FilterPlayers(DriverManager& driver_manager, const std::uintptr_t& client)
{
	// clear playerList
	playerList.clear();

	auto entityList = driver_manager.read_memory<std::uintptr_t>(client + _offset::dwEntityList);

	if (!entityList)
		return;

	auto localPawn = driver_manager.read_memory<std::uintptr_t>(client + _offset::dwLocalPlayerPawn);

	// check swedz video for an explanation of this, i do not have the patience to write out all those comments :(
	for (int i = 0; i <= 64; ++i)
	{
		std::uintptr_t list_entry1 = driver_manager.read_memory<uintptr_t>(entityList + (8 * (i & 0x7FFF) >> 9) + 16);

		std::uintptr_t playerController = driver_manager.read_memory<std::uintptr_t>(list_entry1 + 120 * (i & 0x1FF));

		uint32_t playerPawn = driver_manager.read_memory<uint32_t>(playerController + _offset::m_hPlayerPawn);

		std::uintptr_t list_entry2 = driver_manager.read_memory<std::uintptr_t>(entityList + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);

		std::uintptr_t pCSPlayerPawnPtr = driver_manager.read_memory<std::uintptr_t>(list_entry2 + 120 * (playerPawn & 0x1FF));

		int health = driver_manager.read_memory<int>(pCSPlayerPawnPtr + _offset::m_iHealth);

		if (health <= 0 || health > 100)
			continue;

		int team = driver_manager.read_memory<int>(pCSPlayerPawnPtr + _offset::m_iTeamNum);

		//if (team == driver_manager.read_memory<int>(localPawn + _offset::m_iTeamNum))
			//continue;

		// save the address of the pawn we're on for later use, possibly reading positions.
		CCSPlayerPawn.pCSPlayerPawn = pCSPlayerPawnPtr;

		// push back the entity we're on now, as in save them for later so we can loop through them.
		playerList.push_back(CCSPlayerPawn);
	}
}
inline my_Vector findClosest(const std::vector<my_Vector> playerPositions)
{
	// check if the player positions vector is empty, if it is then just break out of the function.
	if (playerPositions.empty())
	{
		printf("playerPositions vector was empty.\n");
		return { 0.0f, 0.0f, 0.0f };
	}

	// get the center of the screen to be able to subtract the playerPosition by the center of the screen so we know where they are on the screen.
	my_Vector center_of_screen{ (float)GetSystemMetrics(0) / 2, (float)GetSystemMetrics(1) / 2, 0.0f };

	// keep track of the lowest distance found
	float lowestDistance = 10000;

	// find the index of the new lowest distance in the my_Vector and store it (-1 means there wasn't one found)
	int index = -1;

	// loop through every single my_Vector.
	for (int i = 0; i < playerPositions.size(); ++i)
	{
		// at the current index we're at, check the playerPosition and then calculate its distance from the center.
		float distance(std::pow(playerPositions[i].x - center_of_screen.x, 2) + std::pow(playerPositions[i].y - center_of_screen.y, 2));

		// if the distance is lower than the last my_Vector we checked, then add it and save the index.
		if (distance < lowestDistance) {
			lowestDistance = distance;
			index = i;
		}
	}

	// check if we even found a player.
	if (index == -1) {
		return { 0.0f, 0.0f, 0.0f };
	}

	// return the player at that index.
	return { playerPositions[index].x, playerPositions[index].y, 0.0f };
}

inline void MoveMouseToPlayer(my_Vector position)
{
	// check if the position is valid, make a function for this for better practice. this is also just ugly.
	if (position.IsZero())
		return;

	// get the center of our screen.
	my_Vector center_of_screen{ (float)GetSystemMetrics(0) / 2, (float)GetSystemMetrics(1) / 2, 0.0f };

	// get our new x and y, by subtracting the position by the center of the screen, giving us a position to move the mouse to.
	auto new_x = position.x - center_of_screen.x;
	auto new_y = position.y - center_of_screen.y;

	// move the mouse to said position.
	mouse_event(MOUSEEVENTF_MOVE, new_x, new_y, 0, 0);
}
static void aim_bot_(DriverManager& driver_manager, const std::uintptr_t& client)
{

	while (true)
	{
		// sleep for 1ms to save cpu %
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		Reader reader(client);
		reader.FilterPlayers(driver_manager,client);
		// get our view_matrix
		auto view_matrix = driver_manager.read_memory<view_matrix_t>(reader.client + _offset::dwViewMatrix);
		// create our playerPositions vector, although i would recommend moving this out of the loop.
		std::vector<my_Vector> playerPositions;

		// clear our playerPositions vector to remove old players
		playerPositions.clear();
		
		for (const auto& player : reader.playerList)
		{
			// get the 3D position of the player we're CURRENTLY looping through.
			my_Vector playerPosition = driver_manager.read_memory<my_Vector>(player.pCSPlayerPawn + _offset::m_vOldOrigin);

			// create a headPosition my_Vector, this is kind of ghetto but it works fine.
			my_Vector headPos = { playerPosition.x += 0.0, playerPosition.y += 0.0, playerPosition.z += 65.0f };

			// create our out variables for the world_to_screen function.
			my_Vector f, h;

			if (my_Vector::world_to_screen(view_matrix, playerPosition, f) &&
				my_Vector::world_to_screen(view_matrix, headPos, h))
			{
				// add the filtered player to our vector
				playerPositions.push_back(h);
			}
		}

		// check if the user is holding the right mouse button.
		if (GetAsyncKeyState(VK_LSHIFT))
		{
			// find the closest player and store it in a variable
			auto closest_player = findClosest(playerPositions);

			// move the mouse to the player
			MoveMouseToPlayer(closest_player);
		}
	}
}


/*
inline void aim_bot(DriverManager& driver_manager, const std::uintptr_t& client)
{
	while(true)
	{
		std::uintptr_t entityList = driver_manager.read_memory<std::uintptr_t>(
			client + cs2_dumper::offsets::client_dll::dwEntityList);
		if (!entityList)
			return;

		std::uintptr_t localPlayerPawn = driver_manager.read_memory<std::uintptr_t>(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
		if (!localPlayerPawn)
			return;

		BYTE team = driver_manager.read_memory<BYTE>(localPlayerPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

		Vector3 localEyePos = driver_manager.read_memory<Vector3>(localPlayerPawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin) +
			driver_manager.read_memory<Vector3>(localPlayerPawn + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);

		float closest_distance = -1;
		Vector3 enemyPos;

		for (int i = 0; i < 32; i++)
		{
			std::uintptr_t listEntry = driver_manager.read_memory<std::uintptr_t>(entityList + ((8 * (i & 0x7ff)) >> 9) + 16);
			if (!listEntry) continue;

			std::uintptr_t entityController = driver_manager.read_memory<std::uintptr_t>(listEntry + 120 * (i & 0x1ff));
			if (!entityController) continue;

			std::uintptr_t entityControllerPawn = driver_manager.read_memory<std::uintptr_t>(entityController);

			std::uintptr_t entityPawn = driver_manager.read_memory<std::uintptr_t>(listEntry + (entityControllerPawn * (i & 0x1ff)));
			if (!entityPawn) continue;

			if (team == driver_manager.read_memory<BYTE>(entityPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum))
				continue;

			if (driver_manager.read_memory<std::uint32_t>(entityPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth) <= 0)
				continue;

			Vector3 entityEyePos = driver_manager.read_memory<Vector3>(entityPawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin) +
				driver_manager.read_memory<Vector3>(entityPawn + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);

			float current_distance = localEyePos.DistanceTo(entityEyePos); //distance(localEyePos, entityEyePos);

			if (closest_distance < 0 || current_distance < closest_distance)
			{
				closest_distance = current_distance;
				enemyPos = entityEyePos;
			}

			Vector3 relativeAngle = (enemyPos - localEyePos).RelativeAngle();

			driver_manager.write_memory(client + cs2_dumper::offsets::client_dll::dwViewAngles, relativeAngle);

		}
	}
}
*/

/*
void Aimbot::doAimbot(DriverManager& driver_manager, const std::uintptr_t& client)
{
	// sleep for 1ms to save cpu %
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	// get our view_matrix
	auto view_matrix = driver_manager.read_memory<view_matrix_t>(reader.client + offset::dwViewMatrix);

	// create our playerPositions vector, although i would recommend moving this out of the loop.
	std::vector<my_Vector> playerPositions;

	// clear our playerPositions vector to remove old players
	playerPositions.clear();

}
*/