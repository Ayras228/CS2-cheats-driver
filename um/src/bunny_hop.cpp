#pragma once
#include <iostream>
#include "driver_manager.h"
#include"cs2_data/client_dll.hpp"
#include"cs2_data/offsets.hpp"
#include"cs2_data/buttons.hpp"

static void bunny_hop(DriverManager &driver_manager, const std::uintptr_t &client)
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
        const auto flags = driver_manager.read_memory<std::uint32_t>(
            local_player_pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_fFlags);

        const bool in_air = flags & (1 << 0);
        const bool space_pressed = GetAsyncKeyState(VK_SPACE);
        const auto force_jump = driver_manager.read_memory<DWORD>(
            client + cs2_dumper::buttons::jump);

        if (space_pressed && in_air)
        {
            Sleep(5);
            driver_manager.write_memory(client + cs2_dumper::buttons::jump, 65537);
        }
        else if (space_pressed && !in_air)
        {
            driver_manager.write_memory(client + cs2_dumper::buttons::jump, 256);
        }
        else if (!(space_pressed) && force_jump == 65537)
        {
            driver_manager.write_memory(client + cs2_dumper::buttons::jump, 256);
        }
    }

}