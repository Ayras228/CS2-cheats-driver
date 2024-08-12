
#include <iostream>
#include "driver_manager.h"
#include "process_utils.h"

#include"client_dll.hpp"
#include"offsets.hpp"
#include"buttons.hpp"

int main()
{
    std::cout << "Hello world\n";

    DriverManager driver_manager;

    const DWORD pid = get_process_id(L"cs2.exe");

    if (pid == 0)
    {
        std::cout << "Failed to find cs2\n";
        std::cin.get();
        return 1;
    }
    std::cout << "Found cs2\n";

    if (!driver_manager.connect_to_driver(L"\\\\.\\SexyDriver"))
    {
        std::cout << "Failed to create our driver handle.\n";
        std::cin.get();
        return 1;
    }

    if (driver_manager.attach_to_process(pid))
    {
        std::cout << "Attachment successful.\n";
        if (const std::uintptr_t client = get_module_base(pid, L"client.dll"); client != 0)
        {
            std::cout << "Client found.\n";

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
    }

    driver_manager.close_driver();
    std::cin.get();

    return 0;
}
