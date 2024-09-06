#include <iostream>
#include <thread>

#include "driver_manager.h"
#include "process_utils.h"
#include "Bunny_Hop/bunny_hop.cpp"
#include "Aim_Bot/aim_bot.cpp"
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

            // Запуск bunny_hop в окремому потоці
            std::thread bunny_hop_thread(bunny_hop, std::ref(driver_manager), std::ref(client));

           

            //aim_bot(driver_manager, client);
            // Запуск aim_bot в окремому потоці
            //std::thread aim_bot_thread(aim_bot, std::ref(driver_manager), std::ref(client));

            // Основний потік може виконувати інші завдання або чекати завершення додатку
            //aim_bot_thread.join(); // Приєднання потоку до основного (опціонально)
            // Основний потік може виконувати інші завдання або чекати завершення додатку
            //bunny_hop_thread.join(); // Приєднання потоку до основного (опціонально)

            //aim_bot_(driver_manager, client);
             //Запуск aim_bot в окремому потоці
            std::thread aim_bot_thread(aim_bot, std::ref(driver_manager), std::ref(client));

             //Основний потік може виконувати інші завдання або чекати завершення додатку
            aim_bot_thread.join(); // Приєднання потоку до основного (опціонально)

            //Основний потік може виконувати інші завдання або чекати завершення додатку
            bunny_hop_thread.join(); // Приєднання потоку до основного (опціонально)

        }
    }

    driver_manager.close_driver();
    std::cin.get();

    return 0;
}
