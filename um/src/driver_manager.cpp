#include "driver_manager.h"
#include <Windows.h>

DriverManager::DriverManager() : driver_handle(INVALID_HANDLE_VALUE) {}

bool DriverManager::connect_to_driver(const std::wstring& driver_name)
{
    driver_handle = CreateFile(driver_name.c_str(), GENERIC_READ, 0, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    return driver_handle != INVALID_HANDLE_VALUE;
}

void DriverManager::close_driver()
{
    if (driver_handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(driver_handle);
        driver_handle = INVALID_HANDLE_VALUE;
    }
}

bool DriverManager::attach_to_process(const DWORD pid)
{
    driver::Request request;
    request.process_id = reinterpret_cast<HANDLE>(pid);
    return DeviceIoControl(driver_handle, driver::codes::attach,
        &request, sizeof(request), &request, sizeof(request), nullptr, nullptr);
}

//template<class T>
//T DriverManager::read_memory(const std::uintptr_t addr)
//{
//    T temp = {};
//    driver::Request request;
//    request.target = reinterpret_cast<PVOID>(addr);
//    request.buffer = &temp;
//    request.size = sizeof(T);
//
//    DeviceIoControl(driver_handle, driver::codes::read,
//        &request, sizeof(request), &request, sizeof(request), nullptr, nullptr);
//    return temp;
//}
//
//template<class T>
//void DriverManager::write_memory(const std::uintptr_t addr, const T& value)
//{
//    driver::Request request;
//    request.target = reinterpret_cast<PVOID>(addr);
//    request.buffer = (PVOID)&value;
//    request.size = sizeof(T);
//
//    DeviceIoControl(driver_handle, driver::codes::write,
//        &request, sizeof(request), &request, sizeof(request), nullptr, nullptr);
//}
