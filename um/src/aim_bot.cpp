#include <thread> // Для роботи з потоками
#include <windows.h> // Для GetAsyncKeyState

#include <limits> // Додаємо цей заголовок для використання std::numeric_limits
#include <cmath>   // Для std::sqrt і std::pow  
#include <cfloat>  // Для FLT_MAX

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream> // Для типів даних, таких як uint32_t

#include "driver_manager.h"
#include"cs2_data/client_dll.hpp"
#include"cs2_data/offsets.hpp"
#include"cs2_data/buttons.hpp"


// Структура для зберігання координат у тривимірному просторі
struct Vector3
{
    float x, y, z;
};
struct Vector2
{
    float x, y;
};

// Функція для розрахунку кута огляду`
inline Vector2 calculate_view_angles(const Vector3& local_position, const Vector3& enemy_position) {
    Vector3 delta = {
        enemy_position.x - local_position.x,
        enemy_position.y - local_position.y,
        enemy_position.z - local_position.z
    };

    float hypotenuse = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    Vector2 angles;
    angles.x = std::atan2(delta.y, delta.x) * (180.0f / static_cast<float>(M_PI)); // Yaw
    angles.y = std::atan2(delta.z, hypotenuse) * (180.0f / static_cast<float>(M_PI)); // Pitch
    angles.x = 24.0f;
    angles.y = -30.0f;
    return angles;
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
            local_player_pawn + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);

        
        float closest_distance = FLT_MAX;  // Використання визначення з <cfloat>

        Vector3 closest_enemy_position = { 0, 0, 0 };
        bool enemy_found = false;

        // Припустимо, що у вас є офсет для кількості гравців
        const int max_players = 10; // Задайте реальне значення, якщо відомо, або обмежте вручну

        for (int i = 0; i < max_players; ++i)
        {
            const auto enemy_pawn = driver_manager.read_memory<std::uintptr_t>(
                client + cs2_dumper::offsets::client_dll::dwEntityList + i * sizeof(std::uintptr_t));

            if (enemy_pawn == 0 || enemy_pawn == local_player_pawn)
            {
                continue;
            }

            Vector3 enemy_position = driver_manager.read_memory<Vector3>(
                enemy_pawn + cs2_dumper::offsets::client_dll::dwEntityList);

            if (enemy_position.x == 0 && enemy_position.y == 0 && enemy_position.z == 0)
            {
                continue;
            }

            // Розрахунок відстані до ворога
            float distance = std::sqrt(
                std::pow(enemy_position.x - local_position.x, 2) +
                std::pow(enemy_position.y - local_position.y, 2) +
                std::pow(enemy_position.z - local_position.z, 2));

            // Якщо знайдено ближчого ворога
            if (distance < closest_distance)
            {
                closest_distance = distance;
                closest_enemy_position = enemy_position;
                enemy_found = true;
            }
        }

        // Якщо знайдено ворога
        if (enemy_found)
        {
            // Розрахунок кутів огляду для наведення на найближчого ворога
            Vector2 view_angles = calculate_view_angles(local_position, closest_enemy_position);

            // Запис розрахованого кута огляду
            driver_manager.write_memory(client + cs2_dumper::offsets::client_dll::dwViewAngles, view_angles);
        }
    }
}