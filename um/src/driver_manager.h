#pragma once
#include <Windows.h>
#include <string>

namespace driver
{
    namespace codes
    {
        constexpr ULONG attach =
            CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);

        constexpr ULONG read =
            CTL_CODE(FILE_DEVICE_UNKNOWN, 0x697, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);

        constexpr ULONG write =
            CTL_CODE(FILE_DEVICE_UNKNOWN, 0x698, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
    }

    struct Request
    {
        HANDLE process_id;
        PVOID target;
        PVOID buffer;
        SIZE_T size;
        SIZE_T return_size;
    };
}

class DriverManager
{
private:
    HANDLE driver_handle;

public:
    DriverManager();
    bool connect_to_driver(const std::wstring& driver_name);
    void close_driver();
    bool attach_to_process(const DWORD pid);

    template<class T>
    T read_memory(const std::uintptr_t addr)
    {
        T temp = {};
        driver::Request request;
        request.target = reinterpret_cast<PVOID>(addr);
        request.buffer = &temp;
        request.size = sizeof(T);

        DeviceIoControl(driver_handle, driver::codes::read,
            &request, sizeof(request), &request, sizeof(request), nullptr, nullptr);
        return temp;
    }

    template<class T>
    void write_memory(const std::uintptr_t addr, const T& value)
    {
        driver::Request request;
        request.target = reinterpret_cast<PVOID>(addr);
        request.buffer = (PVOID)&value;
        request.size = sizeof(T);

        DeviceIoControl(driver_handle, driver::codes::write,
            &request, sizeof(request), &request, sizeof(request), nullptr, nullptr);
    }
};
