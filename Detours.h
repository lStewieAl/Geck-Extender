#pragma once
namespace Detours
{
	uintptr_t IATHook(uintptr_t Module, const char* ImportModule, const char* API, uintptr_t Detour);
	uintptr_t IATDelayedHook(uintptr_t Module, const char* ImportModule, const char* API, uintptr_t Detour);
}