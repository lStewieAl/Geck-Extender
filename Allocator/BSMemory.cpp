#include "BSMemory.hpp"
#include "MemoryManager.hpp"

void	CreateHeapIfNotExisting(UInt32 auiHeapAddress, UInt32 auiCallAddress);
int(__cdecl* CreateHeap)(UInt32 aeSerialize);
int		CreateHeapStub(UInt32 aeSerialize) { return 1; }
bool	bInitialized = false;

_declspec(noinline) void InitializeHeap() {
	if (*(UInt8*)0x401190 != 0x55) {
		CreateHeap = (int(__cdecl*)(UInt32))0xC770C3;
		CreateHeapIfNotExisting(0xF9907C, 0xC62B21);
	}
	else {
		CreateHeap = (int(__cdecl*)(UInt32))0xEDDB6A;
		CreateHeapIfNotExisting(0x12705BC, 0xECC3CB);
	}

	bInitialized = true;
}

_declspec(noinline)  void* BSNew(size_t stSize) {
	if (!bInitialized)
		InitializeHeap();
	return MemoryManager::GetSingleton()->Allocate(stSize);
}

void* BSNewAligned(size_t stAlign, size_t stSize) {
	char* pMemory = static_cast<char*>(MemoryManager::GetSingleton()->Allocate(stSize + stAlign));
	UInt32 uiAlignment = stAlign - (reinterpret_cast<UInt8>(pMemory) & (stAlign - 1));
	pMemory[UInt8(uiAlignment) - 1] = uiAlignment;
	return &pMemory[UInt8(uiAlignment)];
}

void* BSReallocate(void* pvMem, size_t stSize) {
	return MemoryManager::GetSingleton()->Reallocate(pvMem, stSize);
}

void BSFree(void* pvMem) {
	MemoryManager::GetSingleton()->Deallocate(pvMem);
}

SIZE_T BSSize(void* pvMem) {
	return MemoryManager::GetSingleton()->Size(pvMem);
}

// -------------------------------------------------------------------------
// Functions made to initialize the allocator
// Compatible with both game and GECK
// -------------------------------------------------------------------------

// This function is used to create game's heap if it doesn't exist
// It's possible to load the plugin before game is even initialized
// In those cases, malloc fails due to lack of heap - that's why we need to create it manually
_declspec(noinline) void CreateHeapIfNotExisting(UInt32 auiHeapAddress, UInt32 auiCallAddress) {
	if (*(HANDLE*)auiHeapAddress)
		return;

	CreateHeap(true);

	UInt32 oldProtect;
	VirtualProtect((void*)auiCallAddress, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*(UInt32*)(auiCallAddress + 1) = UInt32(CreateHeapStub) - auiCallAddress - 5;
	VirtualProtect((void*)auiCallAddress, 4, oldProtect, &oldProtect);
}