#pragma once
#include <Windows.h>
#include <iostream>

DWORD get_process_id(const wchar_t* process_name);
std::uintptr_t get_module_base(const DWORD pid, const wchar_t* module_name);
